!function () {
	//
	var sb, app_name, app_description,
		runSetup, onOpen, onMessage;
	//
	runSetup  = function () {

		app_name = 'Spacebrew/YÚN Web Control';

		app_desc = 'Spacebrew/YÚN Web Controller demonstration';

		sb = new Spacebrew.Client('nowir.es', app_name, app_description, {reconnect: true});

		sb.addPublish('LED-ON', 'boolean', 'true');

		sb.addPublish('LED-OFF', 'boolean', 'false');

		sb.addSubscribe('Receive Message', 'string');

		sb.onStringMessage = onMessage;

		sb.onOpen = onOpen;

		sb.connect();
	};
	//
	onOpen = function () {
		$('#connection_display').html('Connected as ' + sb.name());
	}
	//
	onMessage = function (name, value) {
		if(value){
			$('#message_display').text(value);
		}
	}
	//
	$(function() {

    	runSetup();

    	$('#btn_led_on').bind('click', function () {
    		sb.send('LED-ON', 'boolean', 'true');
    	});

    	$('#btn_led_off').bind('click', function () {
    		sb.send('LED-OFF', 'boolean', 'false');
    	});

	});
}();