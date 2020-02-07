/*jshint esversion: 8 */
function log(logstr) {
  document.getElementById("js-output").innerHTML += logstr + "\n";
}

/**
 * This function is used to give approval that a token may be transfered from your wallet to the chosen contract address.
 * The Contract information variables are found in contract-info.js, change these according to your own contract.
 */
async function callApprove() {
    web3 = new Web3(Web3.givenProvider);
    web3.extend({ // web3.eth.requestAccounts() isn't available (yet)
        methods: [{
            name: 'eth_requestAccounts',
            call: 'eth_requestAccounts',
            params: 0
        }]
    });
    var clientAddress = await web3.eth_requestAccounts().catch(x=>log(x.message));
    
    const tokenContract = new web3.eth.Contract(TOKEN_CONTRACT_ABI, TOKEN_CONTRACT_ADDRESS);
    tokenContract.methods.approve(ARCADE_CONTRACT_ADDRESS, APPROVE_VALUE).send({from: `${clientAddress}`})
    .then(function(receipt){
      console.log(receipt);
      callAddGameCoin();
    });
}

/**
 * This function transfers the token from the owner to the contract, within the contract, the transaction will be confirmed with an emit.
 * The Contract information variables are found in contract-info.js, change these according to your own contract.
 */
async function callAddGameCoin(){
    web3 = new Web3(Web3.givenProvider);
    web3.extend({ // web3.eth.requestAccounts() isn't available (yet)
        methods: [{
            name: 'eth_requestAccounts',
            call: 'eth_requestAccounts',
            params: 0
        }]
    });
    var clientAddress = await web3.eth_requestAccounts().catch(x=>log(x.message));

    const arcadeContract = new web3.eth.Contract(ARCADE_CONTRACT_ABI, ARCADE_CONTRACT_ADDRESS);
    arcadeContract.methods.addGameCoin().send({from: `${clientAddress}`});
}

/**
 * Used for web3connect to choose which wallet to connect to the site.
 */
async function web3connector() {
    const web3Connect = new Web3Connect.Core({
        network: "mainnet", // optional
        providerOptions: {
            walletconnect: {
                package: WalletConnectProvider,
                    options: { infuraId: "0" } // dummy infura code!!
            }
        }
    });
    web3Connect.toggleModal();
    web3Connect.on("connect", OnConnect);
}

/**
 * Triggers when a provider wallet is chosen
 * @param {any} provider 
 */
async function OnConnect(provider) {
    const web3 = new Web3(provider); // add provider to web3
    var acts=await web3.eth.getAccounts().catch(log);
    log(`This is the currently connected account: ${JSON.stringify(acts)}`);
}

/**
 * Due to issues with calling an asynchronous function from HTML this is used as a workaround
 */
function f() {
    callApprove(); 
}  

web3connector();
