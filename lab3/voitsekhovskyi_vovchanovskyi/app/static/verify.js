$(document).ready(function(){

  var base64String;
  var publicKey;
  var documentfile;
  var documenthash;
  var signature;

  function base64ToArrayBuffer(base64) {
    var binary_string = window.atob(base64);
    var len = binary_string.length;
    var bytes = new Uint8Array(len);
    for (var i = 0; i < len; i++) {
        bytes[i] = binary_string.charCodeAt(i);
    }
    return bytes.buffer;
  }


  $('#signature').on('change', function () {
    var f = new FileReader();
    f.onload = function () {
    var data = f.result;
    base64String = data;  // data <-- in this var you have the file data in Base64 format
    };
    f.readAsText($(this).prop('files')[0]);

  });
  // Read private key
  $('#key').on('change', function () {
    var f = new FileReader();
    f.onload = function () {
    var data = f.result;
    pemEncodedKey = data;  // data <-- in this var you have the file data in Base64 format
    };
    f.readAsText($(this).prop('files')[0]);

  });

  // Read document to sign
  $('#document').on('change', function () {
    var f = new FileReader();
    f.onload = function () {
    var data = f.result;
    documentfile = data;  // data <-- in this var you have the file data in Base64 format
    };
    f.readAsText($(this).prop('files')[0]);

  });

  /*
  Convert a string into an ArrayBuffer
  from https://developers.google.com/web/updates/2012/06/How-to-convert-ArrayBuffer-to-and-from-String
  */
  function str2ab(str) {
    const buf = new ArrayBuffer(str.length);
    const bufView = new Uint8Array(buf);
    for (let i = 0, strLen = str.length; i < strLen; i++) {
      bufView[i] = str.charCodeAt(i);
    }
    return buf;
  }
  function arrayBufferToBase64String(arrayBuffer) {
      var byteArray = new Uint8Array(arrayBuffer)
      var byteString = ''
      for (var i=0; i<byteArray.byteLength; i++) {
        byteString += String.fromCharCode(byteArray[i])
      }
      return btoa(byteString)
  }
  /*
  Import a PEM encoded RSA private key, to use for RSA-PSS signing.
  Takes a string containing the PEM encoded key, and returns a Promise
  that will resolve to a CryptoKey representing the private key.
  */
  function importPublicKey(pem) {
    // fetch the part of the PEM string between header and footer
      const pemHeader = "-----BEGIN PUBLIC KEY-----";
      const pemFooter = "-----END PUBLIC KEY-----";
      const pemContents = pem.substring(pemHeader.length, pem.length - pemFooter.length-1);
      console.log(pemContents);
      // base64 decode the string to get the binary data
      const binaryDerString = window.atob(pemContents);
      // convert from a binary string to an ArrayBuffer
      const binaryDer = str2ab(binaryDerString);

      return window.crypto.subtle.importKey(
        "spki",
        binaryDer,
        {
          name: "RSA-PSS",
          hash: "SHA-256"
        },
        true,
        ["verify"]
      );
  }
  /*
  And encode it in a form we can use for the sign operation.
  */
  function getMessageEncoding() {
    const enc = new TextEncoder();
    return enc.encode(documenthash);
  }

  async function verifyMessage(publicKey) {
    let encoded = getMessageEncoding();
    let result = await window.crypto.subtle.verify(
      {
        name: "RSA-PSS",
        saltLength: 32,
      },
      publicKey,
      signature,
      encoded
    );

    res = document.getElementById("result")
    if(result) {
      res.innerHTML = "True"
      res.classList = ["text-success"];
    } else {
      res.innerHTML = "False"
      res.classList = ["text-danger"];
    }
  }

  async function digestMessage(message) {
    const msgUint8 = new TextEncoder().encode(message);                           // encode as (utf-8) Uint8Array
    const hashBuffer = await crypto.subtle.digest('SHA-256', msgUint8);           // hash the message
    const hashArray = Array.from(new Uint8Array(hashBuffer));                     // convert buffer to byte array
    const hashHex = hashArray.map(b => b.toString(16).padStart(2, '0')).join(''); // convert bytes to hex string
    return hashHex;
  }
  $('#verify').on('click', async () => {
    console.log(pemEncodedKey);
    console.log(base64String);
    documenthash = await digestMessage(documentfile);
    publicKey = await importPublicKey(pemEncodedKey);
    signature = await base64ToArrayBuffer(base64String)
    console.log(publicKey);
    verifyMessage(publicKey);

  });
});
