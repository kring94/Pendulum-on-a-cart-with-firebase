const loginElement = document.querySelector('#login-form');
const contentElement = document.querySelector("#content-sign-in");
const userDetailsElement = document.querySelector('#user-details');
const authBarElement = document.querySelector("#authentication-bar");

// Elements for reading data from database
const modeActionText = document.getElementById("mode_action");

// Labels for PID control
const kpElement = document.getElementById("kp_label");
const kiElement = document.getElementById("ki_label");
const kdElement = document.getElementById("kd_label");

const inputkpElement = parseFloat(kpElement.value);
const inputkiElement = parseFloat(kiElement.value);
const inputkdElement = parseFloat(kdElement.value);

// Elements for sensor readings
const angleElement = document.getElementById("angle");
const humElement = document.getElementById("hum");
const presElement = document.getElementById("pres");
const modeElement = document.getElementById("mode");
const stateElement = document.getElementById("pendulum_state");

// Element for mode and state 
var dbPathMode;
var dbPathState;

var dbPathKI;
var dbPathKP;
var dbPathKD;

var dbPathUp;
var dbPathLeft;
var dbPathStop;
var dbPathRight;
var dbPathDown;


// MANAGE LOGIN/LOGOUT UI
const setupUI = (user) => {
  if (user) {
    //toggle UI elements
    loginElement.style.display = 'none';
    contentElement.style.display = 'block';
    authBarElement.style.display ='block';
    userDetailsElement.style.display ='block';
    userDetailsElement.innerHTML = user.email;

    // get user UID to get data from database
    var uid = user.uid;
    console.log(uid);

    // Database paths (with user UID)
    var dbPathAngle = 'UsersData/' + uid.toString() + '/angle';
    var dbPathSetPoint = 'UsersData/' + uid.toString() + '/SetPoint';
    //var dbPathHum = 'UsersData/' + uid.toString() + '/humidity';
    //var dbPathPres = 'UsersData/' + uid.toString() + '/pressure';

    dbPathMode = 'UsersData/' + uid.toString() + '/mode';
    dbPathState = 'UsersData/' + uid.toString() + '/state';

    dbPathKI = 'UsersData/' + uid.toString() + '/ki';
    dbPathKP = 'UsersData/' + uid.toString() + '/kp';
    dbPathKD = 'UsersData/' + uid.toString() + '/kd';
    // Set path for buttom of the manual control
    dbPathUp = 'UsersData/' + uid.toString() + '/up';
    dbPathLeft = 'UsersData/' + uid.toString() + '/left';
    dbPathStop = 'UsersData/' + uid.toString() + '/stop';
    dbPathRight = 'UsersData/' + uid.toString() + '/right';
    dbPathDown = 'UsersData/' + uid.toString() + '/down';
    // Set value of path in mode OFF
    firebase.database().ref(dbPathUp).set("OFF");
    firebase.database().ref(dbPathLeft).set("OFF");
    firebase.database().ref(dbPathStop).set("ON");
    firebase.database().ref(dbPathRight).set("OFF");
    firebase.database().ref(dbPathDown).set("OFF");
    firebase.database().ref(dbPathMode).set("OFF");
    firebase.database().ref(dbPathState).set("OFF");


    // Database references
    var dbRefAngle = firebase.database().ref().child(dbPathAngle);

    // Update page with new readings
    dbRefAngle.on('value', snap => {

      angleElement.innerText = snap.val().toFixed(2);
      var x = (new Date()).getTime(),
      y= parseFloat(snap.val().toFixed(2));

         // y = parseFloat(this.responseText);
      //console.log(this.responseText);
      if(chartA.series[0].data.length > 40) {
        chartA.series[0].addPoint([x, y], true, true, true);
      } else {
        chartA.series[0].addPoint([x, y], true, false, true);
      }
    
    });

  // if user is logged out
  } else{
    // toggle UI elements
    loginElement.style.display = 'block';
    authBarElement.style.display ='none';
    userDetailsElement.style.display ='none';
    contentElement.style.display = 'none';
  }
}

function toggleMode() {
  console.log("Toggle");
  if (modeElement.checked) 
  {
    console.log("Mode control pendulum");
    firebase.database().ref(dbPathMode).set("ON");
    modeActionText.innerText = "-Pendulum";
    //Activación o desactivación según modo
    document.getElementById("control_car").style.display = "none";
    document.getElementById("control_pendulum").style.display = "block";
    document.getElementById("charts-div").style.display = "block";
    // Set value of Button for default
    firebase.database().ref(dbPathUp).set("OFF");
    firebase.database().ref(dbPathLeft).set("OFF");
    firebase.database().ref(dbPathStop).set("ON");
    firebase.database().ref(dbPathRight).set("OFF");
    firebase.database().ref(dbPathDown).set("OFF");

  }
  else{
    console.log("Mode control manual");
    firebase.database().ref(dbPathMode).set("OFF");
    firebase.database().ref(dbPathState).set("OFF");// Desactivación del estado activo del pendulo
    stateElement.checked = false;
    modeActionText.innerText = "-Manual Car";
    //Activación o desactivación según modo
    document.getElementById("control_car").style.display = "block";
    document.getElementById("control_pendulum").style.display = "none";
    document.getElementById("charts-div").style.display = "none";

  }
}

function toggleState() {
  console.log("Toggle");
  if (stateElement.checked) 
  {
    console.log("State Pendulum: ON");
    firebase.database().ref(dbPathState).set("ON");

  }
  else{
    console.log("State Pendulum: OFF");
    firebase.database().ref(dbPathState).set("OFF");

  }
}

// Funtion for PID control

function kpCommand() {  
  console.log(parseFloat(kpElement.value));
  if(!isNaN(parseFloat(kpElement.value))){
    firebase.database().ref(dbPathKP).set(parseFloat(kpElement.value));
  }  
}

function kiCommand() {
  console.log(parseFloat(kiElement.value));
  if(!isNaN(parseFloat(kiElement.value))){
    firebase.database().ref(dbPathKI).set(parseFloat(kiElement.value));
  }  
}

function kdCommand() {
  console.log(parseFloat(kdElement.value));
  if(!isNaN(parseFloat(kdElement.value))){
    firebase.database().ref(dbPathKD).set(parseFloat(kdElement.value));
  }  
}

// Funtion for manul control

function upCommand() {
  firebase.database().ref(dbPathUp).set("ON");
  firebase.database().ref(dbPathLeft).set("OFF");
  firebase.database().ref(dbPathStop).set("OFF");
  firebase.database().ref(dbPathRight).set("OFF");
  firebase.database().ref(dbPathDown).set("OFF");
  
}

function leftCommand() {
  firebase.database().ref(dbPathUp).set("OFF");
  firebase.database().ref(dbPathLeft).set("ON");
  firebase.database().ref(dbPathStop).set("OFF");
  firebase.database().ref(dbPathRight).set("OFF");
  firebase.database().ref(dbPathDown).set("OFF");

}

function stopCommand() {
  firebase.database().ref(dbPathUp).set("OFF");
  firebase.database().ref(dbPathLeft).set("OFF");
  firebase.database().ref(dbPathStop).set("ON");
  firebase.database().ref(dbPathRight).set("OFF");
  firebase.database().ref(dbPathDown).set("OFF");
}

function rightCommand() {
  firebase.database().ref(dbPathUp).set("OFF");
  firebase.database().ref(dbPathLeft).set("OFF");
  firebase.database().ref(dbPathStop).set("OFF");
  firebase.database().ref(dbPathRight).set("ON");
  firebase.database().ref(dbPathDown).set("OFF");

}

function downCommand() {
  firebase.database().ref(dbPathUp).set("OFF");
  firebase.database().ref(dbPathLeft).set("OFF");
  firebase.database().ref(dbPathStop).set("OFF");
  firebase.database().ref(dbPathRight).set("OFF");
  firebase.database().ref(dbPathDown).set("ON");

}


/*setInterval(function ( ) {
 
      var x = (new Date()).getTime(),
      y=5;
         // y = parseFloat(this.responseText);
      //console.log(this.responseText);
      if(chartT.series[0].data.length > 40) {
        chartT.series[0].addPoint([x, y], true, true, true);
      } else {
        chartT.series[0].addPoint([x, y], true, false, true);
      }
    
 
 
}, 1000 ) ;*/