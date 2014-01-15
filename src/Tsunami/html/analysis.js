Analysis = function( _names ){
	'use strict';	
	var _sources = [];
	var _id = -1;
	var _modes   = [{key: 'voltage', value:'Напряжение'},
		 		    {key: 'current', value:'Ток'},
		 		    {key: 'gnd',     value:'Земля'}];
	var _methods = [{key: 'const',   value:'Константа'},
				    {key: 'linear',  value:'Линейный'}]

    this.new = function(){
    	create();
    }

    this.init = function( id,sources ){
		console.log( 'Initialization analysis',id );
		if(id == -1){
			create();
		}else{
			_sources = sources;
		}

		var nodeNames = Object.keys(_sources);
		_id = id;
		
		hiddenNode();
		
		$.each(nodeNames, function(_,node){
			showNode(node);
		});

		$('input[name="save"]').unbind('click',save);
		$('input[name="save"]').bind('click',save);
	}

	function save() {
		var checkedSave = checkConditions();
		console.log('Analysis id',_id);
		console.log('Conditions', checkedSave.result, checkedSave.error);
		Api.saveAnalysis(_sources);
	}

	this.connect = function( func ){
		try{
			func.disconnect(this.init);
		}catch(err){
			console.log('disconnect ok');
		}
		func.connect( this.init );
		console.log("Connected function",func);
	}

	function create(){
		$.each(_names,function(node,_){
    		_sources[node] = {'mode':'gnd','method':'','config':{}};
    		// showNode(node);
    	});
	}

	function hiddenNode(){
		$('div[node="-"]').each( function(_,obj){
		 	$(obj).hide();
		});
	}
	function prepareNode(node){
		console.log('Prepare node',node);
		var source = _sources[node];

		var nodeObject = $('div[node="'+node+'"]');
		$(nodeObject).find('select[name="mode"]').val(_sources[node].mode);
		$(nodeObject).find('select[name="method"]').val(_sources[node].method );
		
		changeMode( node, source.mode );			
	}
	function configElement( title, key, value ) {
		console.log('Config element',title,key,value);
		if(value == undefined){
			value = 0.0;
		}
		return title+'<br/><input class="cfg" name="'+key+'" value="'+value+'">';
	}
	function selectElement( name, array, selected ){
		var element = '<select name="'+name+'">';
		$.each(array,function(_,item){
			element += '<option value="'+item.key+'"';
			if(item.key == selected){
				element += ' selected="selected"';
			}
			element += '>'+item.value+'</option>';
		});
		element += '</select>';
		return element;
	}

	function showNode(node){
		var source = _sources[node];
		var nodeDiv = $('div[node="'+node+'"]');

		$(nodeDiv).html('');
		$(nodeDiv).append('<strong>'+_names[node]+'</strong><br/>');

		$(nodeDiv).append('Режим:'+selectElement('mode',   _modes,   source.mode)+'<br/>');
							
		if( source.mode != 'gnd' ) {
			$(nodeDiv).append('Метод:'+selectElement('method', _methods, source.method)+'<br/>');
			if(source.method == '' || source.method == undefined){ 
				source.method = 'const';
				source.config["const"] = 0;
			}
			// if(source.config)

			if( source.method == 'const' ){
				$(nodeDiv).append(configElement('Константа', 'const', source.config["const"]));
			} else if( source.method == 'linear' ) {
				var numbers = 'Номер:';

				numbers += '<select class="cfg" name="number">';
				numbers += '<option value="1">1</option>';
				numbers += '<option value="2">2</option>';
				numbers += '</select><br/>';
				$(nodeDiv).append(numbers);
				if(source.config["number"] != undefined){
					$(nodeDiv).find('select[name="number"]').val(source.config["number"]);
				}
				$(nodeDiv).append(configElement('Начало', 'start', source.config["start"]) + '<br/>');
				$(nodeDiv).append(configElement('Шаг',    'step',  source.config["step"])  + '<br/>');
				$(nodeDiv).append(configElement('Стоп',   'end',   source.config["end"])   + '<br/>');
			}
		}

		bindHandler(node);
	}

	function numberLinearSource(){
		var nLinearSource = 0;
		$('select[name="method"]').each( function(_,obj){
			if($(obj).val() == 'linear'){
				nLinearSource++;
			}
		});
		return nLinearSource;
	}
	function conditionError(result,error){
		return {
				'result':result,
				'error':error
			   };
	}
	function checkConditions(){
		

		// Count linear method
		var nLinearSource = numberLinearSource();

		if(nLinearSource > 2){
			return conditionError(false,'Many linear sources');
		}
		if(nLinearSource == 0){
			return conditionError(false,'Empty linear sources');
		}

		// Check number source
		if(nLinearSource == 2){
			var numberSource = 0;
			$('select[name="method"]').each( function(_,obj){
				if($(obj).val() == numberSource){
					return conditionError(false,'Eq number source');
				}else{
					numberSource = $(obj).val();
				}
				if($(obj).val() == undefined){
					return conditionError(false,'undefined number source');
				}
			});
		}

		return conditionError(true,' ');
	}
	function bindHandler(node) {
		var nodeDiv = $('div[node="'+node+'"]');		
		$(nodeDiv).find('select[name="mode"]').each(function(i, obj){	
			$(obj).on('change', function() {
				
				var node = $(obj).parent().attr('node');
				var mode = $(this).val();
				_sources[node].mode = mode;
				showNode(node);
			});
		});
		$(nodeDiv).find('select[name="method"]').each( function(i, obj){
			$(obj).on('change', function(){
				var node = $(obj).parent().attr('node');
				var method = $(this).val();

				// changeMethod( node, method );
				_sources[node].method = method;
				showNode(node);
				
			})
		});
		$(nodeDiv).find('.cfg').each( function(i, obj){
			$(obj).on('change', function(){
				var node = $(obj).parent().attr('node');
				var item = $(this).attr('name');
				var value = $(this).val();
				changeConfig( node, item, value );
				
			})
		});
	}

	function changeConfig( node, item, value ){
		console.log('Change config',node,item,value);
		var source = _sources[node];
		switch( item ){
			case 'number' : source.config["number"] = value; break;
			case 'const':   source.config["const"] = value; break;
			case 'start':   source.config["start"] = value; break;
			case 'step':    source.config["step"] = value; break;
			case 'end':     source.config["end"] = value; break;
			default:
				return;
		}
	}

}