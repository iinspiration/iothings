import React, { Component } from 'react';
import logo from './img/device.png';
import './App.css';

import firebase from 'firebase'
import firebaseConfig from './shared/configs/firebase'
import { FirebaseAPI } from './shared/utils/firebaseUtils'

firebase.initializeApp(firebaseConfig)
FirebaseAPI.get({ path: `Devicename` })
          .then((snapshot) => {
            console.log(snapshot.val())
          })
          .catch((error) => {
            console.log(error)
          })
class App extends Component {

  render() {
    return (
      <div className="App">
        <div className="App-header">
          <img src={logo} className="App-logo" alt="logo" />
          <h2>myHome</h2>
        </div>
      </div>
    );
  }
}

export default App;
