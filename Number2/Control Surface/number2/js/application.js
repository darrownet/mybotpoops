var sb, app_name, app_description,
    runSetup, onOpen, onMessage,
    calculateMovement, getScaledValue;

//
//
//
runSetup  = function () {

  app_name = 'Control Surface';

  app_desc = 'Control Surface for Number 2 the pooping Robot';

  sb = new Spacebrew.Client('nowir.es', app_name, app_description, {reconnect: true});

  sb.addPublish('make', 'boolean', 'false');
  //sb.addPublish('move A', 'range', 0);
  //sb.addPublish('move B', 'range', 0);
  sb.addPublish('move', 'string', '0|0')
  sb.addPublish('set direction', 'string', 'forward');

  sb.addSubscribe('Receive Message', 'string');

  sb.onStringMessage = onMessage;

  sb.onOpen = onOpen;

  sb.connect();
};
//
onOpen = function () {
  $('#display').html('Connected as ' + sb.name());
}
//
onMessage = function (name, value) {
  if(value){
    $('#display').text(value);
  }
}
//
//
//
calculateMovement = function (evt){
  var dir, motor_a, motor_b, power_a, power_b, ver_pos, hor_pos, max_power;
  //
  ver_pos = $(evt.target).position().top + 25;
  hor_pos = $(evt.target).position().left + 25;
  //
  //
  if (ver_pos < 150) {
    dir = 'FORWARD';
    max_power = 255;
  } else if (ver_pos > 150) {
    dir = 'BACKWARD';
    max_power = 255;
  } else {
    dir = 'STOPPED';
  }
  //
  //
  if (hor_pos <  100) {
    power_a = max_power;
    power_b = Math.round(getScaledValue(hor_pos, 100, 25, max_power, 0, true));
  } else if (hor_pos > 200) {
    power_a = Math.round(getScaledValue(hor_pos, 200, 275, max_power, 0, true));
    power_b = max_power;
  } else {
    power_a = max_power;
    power_b = max_power;
  }
  //
  // 
  if (dir === 'FORWARD') {
    motor_a = Math.round(getScaledValue(ver_pos, 150, 25, 0, power_a, true));
    motor_b = Math.round(getScaledValue(ver_pos, 150, 25, 0, power_b, true));
    sb.send('set direction', 'string', 'forward');
  } else if (dir === 'BACKWARD') {
    motor_a = Math.round(getScaledValue(ver_pos, 155, 205, 0, power_a, true));
    motor_b = Math.round(getScaledValue(ver_pos, 155, 205, 0, power_b, true));
    sb.send('set direction', 'string', 'backward');
  } else{
    dir = 'STOPPED';
    sb.send('set direction', 'string', 'stopped');
  }
  if (motor_a === undefined || motor_b === undefined) {
    motor_a = 0;
    motor_b = 0;
  }
  // sb.send('move A', 'range', motor_a);
  // sb.send('move B', 'range', motor_b);
  sb.send('move', 'string', motor_a + '|' + motor_b);
  //
  $("#display").html(dir + '<br />motor-A: ' + motor_a + ' | motor-B ' + motor_b);
};
//
getScaledValue = function ( val, inMin, inMax, outMin, outMax, limiter ) {
  var inRange = inMax - inMin;
  var outRange = outMax - outMin;
  var normalVal = (val - inMin) / inRange;
  if (limiter) {
    normalVal = Math.max(0, Math.min(1, normalVal));
  }
  return outRange * normalVal + outMin;
};
//
//
//
$(function() {
  //
  // Set Up Dragger
  $("#draggable").draggable({
    revert: true,
    revertDuration: 10,
    containment: "parent",
    stop: calculateMovement
  });
  $("#draggable").bind('drag', calculateMovement);
  //
  // Set Up Button;
  $("#poop_btn").bind('click', function (){
    sb.send('make', 'boolean', true);
  });
  //
  //
  //
  // Initialze Spacebrew
  runSetup();
});