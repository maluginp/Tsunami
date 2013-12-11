Analysis = function(){
	
	this.sources;


	this.init = function( sources ){
		this.sources = sources;

		// console.log( 'Initialization', sources.size() );

		for(node in this.sources){
			console.log( "Node", node, "Mode",this.sources[node].mode,"Method",this.sources[node].method );
		 	nodeObject = $('#device div[node="'+node+'"]');
		 	nodeObject.find('select[name="mode"]').val(this.sources[node].mode);
		 	nodeObject.find('select[name="method"]').val( this.sources[node].method );
		 	changeMode( node, this.sources[node].mode );

		}
	}

	function changeMode( node, mode ){
		console.log( 'Start changeMode',node,mode);

		nodeObject = $('#device div[node="'+node+'"]');
		this.sources[ node ].mode = mode;

		var method;
		if( mode == "gnd"){
			method = "";
			nodeObject.find('select[name="method"]').attr('disabled','disabled');
		}else{
			method = nodeObject.find('select[name="method"]').val();
			nodeObject.find('select[name="method"]').removeAttr('disabled');
		}

		console.log( 'changeMode',node, mode );

		changeMethod(node, method);

	}
	this.onChangeMode = function (node,mode){
		changeMode(node,mode);
	}

	function changeMethod( node, method ){
		console.log( 'Start changeMethod',node,'Method',method );

		this.sources[node].method = method;
		nodeObject = $('#device div[node="'+node+'"]');
		if(method == ""){
			nodeObject.find('label').each( function(i, labelObject){
				$(labelObject).hide();
			});
		}else if(method == "const"){
			nodeObject.find('label[name!="const"]').each( function(i, labelObject){
				$(labelObject).hide();
			});
			nodeObject.find('label[name="const"]').show();
			nodeObject.find('label[name="const"] input').val( this.sources[node].config.const );
		}else if(method == "linear"){
			nodeObject.find('label[name="const"]').hide();
			nodeObject.find('label[name!="const"]').each( function(i, labelObject){
				$(labelObject).show();
			});
			nodeObject.find('label[name="start"] input').val( this.sources[node].config.start );
			nodeObject.find('label[name="step"] input').val( this.sources[node].config.step );
			nodeObject.find('label[name="end"] input').val( this.sources[node].config.end );
		}

		console.log('changeMethod', node, method);
	}

	this.save = function(){
		console.log('Click save');
		console.log(sources);
		Api.saveAnalysis(sources);
	}

	this.onChangeMethod = function(node,method){
		changeMethod(node,method);
	}

	this.onChangeConfig = function( node, item, value ){
		nodeObject = $('#device div[node="'+node+'"]');
		switch( item ){
			case 'const': this.sources[node].config.const = value; break;
			case 'start': this.sources[node].config.start = value; break;
			case 'step':  this.sources[node].config.step = value; break;
			case 'end':  this.sources[node].config.end = value; break;
			default:
				return;
		}
		console.log('changeConfig',node,item,value);
		// nodeObject.find("label")
	}

}