Analysis = function(){
	'use strict';	
	this._sources = {};


	this.init = function( sources ){
		console.log( 'Initialization');

		nSources = Object.keys(sources).length;
		console.log( 'Length', nSources );

		App = window.analysis;


		
		if( nSources == 0){
			console.log( 'null' );
			nodes = $( "div[node]" );
			var i;
			for(i=0; i < nodes.length; ++i){
				if( $( nodes[i] ).attr('node') != '-' ){
					console.log( 'Init', $( nodes[i] ).attr('node'));
					App._sources[ $( nodes[i] ).attr('node')  ] = { "mode": "gnd", "method":"const", "config" : {} };
				}
			}

			// console.log('New sources', _sources);
		}else{
			App._sources = sources;
		}
		$('#device div[node="-"]').each( function(i,obj){
			$(obj).hide();
		});
		
		for(node in App._sources){
			console.log( "Node", node, "Mode",App._sources[node].mode,"Method",App._sources[node].method );
		 	nodeObject = $('#device div[node="'+node+'"]');
		 	nodeObject.find('select[name="mode"]').val(App._sources[node].mode);
		 	nodeObject.find('select[name="method"]').val( App._sources[node].method );
		 	console.log("pre changeMode");
		 	App.changeMode( node, App._sources[node].mode );

		}

		


	}

	this.changeMode = function( node, mode ){
		console.log( 'Start changeMode',node,mode);

		App = window.analysis;

		nodeObject = $('#device div[node="'+node+'"]');
		App._sources[ node ].mode = mode;

		var method;
		if( mode == "gnd"){
			method = "";
			nodeObject.find('select[name="method"]').attr('disabled','disabled');
		}else{
			method = nodeObject.find('select[name="method"]').val();
			nodeObject.find('select[name="method"]').removeAttr('disabled');
		}

		// console.log( 'changeMode',node, mode );

		App.changeMethod(node, method);

	}
	this.onChangeMode = function (node,mode){
		this.changeMode(node,mode);
	}

	this.changeMethod = function( node, method ){
		console.log( 'Start changeMethod',node,'Method',method );
		App = window.analysis;

		App._sources[node].method = method;
		nodeObject = $('#device div[node="'+node+'"]');
		if(method == ""){
			nodeObject.find('label').each( function(i, labelObject){
				$(labelObject).hide();
			});
		}else if(method == "const"){
			nodeObject.find('label[method="const"]').each( function(i, obj){
				$(obj).show();
			});
			nodeObject.find('label[method!="const"]').each( function(i, obj){
				$(obj).hide();
			});

			nodeObject.find('label[name="const"] .config').val( App._sources[node].config.const );
		}else if(method == "linear"){
			nodeObject.find('label[method="linear"]').each( function(i, obj){
				$(obj).show();
			});
			nodeObject.find('label[method!="linear"]').each( function(i, obj){
				$(obj).hide();
			});
		
			nodeObject.find('label[name="start"] .config').val( App._sources[node].config.start );
			nodeObject.find('label[name="step"] .config').val( App._sources[node].config.step );
			nodeObject.find('label[name="end"] .config').val( App._sources[node].config.end );
			nodeObject.find('label[name="number"] .config').val( App._sources[node].config.number );
		}

		console.log('changeMethod', node, method);
	}

	this.save = function(){
		console.log('Click save');
		App = window.analysis;
		Api.saveAnalysis(App._sources);
	}

	this.onChangeMethod = function(node,method){
		App = window.analysis;
		App.changeMethod(node,method);
	}

	this.onChangeConfig = function( node, item, value ){
		App = window.analysis;
		console.log( "onChangeConfig", node, item, value );
		nodeObject = $('#device div[node="'+node+'"]');
		switch( item ){
			case 'number' : App._sources[node].config["number"] = value; break;
			case 'const': App._sources[node].config["const"] = value; break;
			case 'start': App._sources[node].config["start"] = value; break;
			case 'step':  App._sources[node].config["step"] = value; break;
			case 'end':  App._sources[node].config["end"] = value; break;
			default:
				return;
		}
		
		// nodeObject.find("label")
	}

}