/*jshint esversion: 8 */
function log(logstr) {
  document.getElementById("js-output").innerHTML += logstr + "\n";
}

const ensName = 'web3testmathieu.eth';

async function getContractAddress(_name) {                           //Function to lookup address with ens name
    var address = await web3.eth.ens.getAddress(_name);
    return address;
}


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




async function versionAndBlock() {
    web3 = new Web3(Web3.givenProvider);
    log(`Version of web3.js: ${web3.version}`);
    var BlockNumber = await web3.eth.getBlockNumber();
    log('blocknumber = ' + BlockNumber);
}

async function balanceOfContract() {
  web3 = new Web3(Web3.givenProvider);
  const contract = new web3.eth.Contract(TOKEN_CONTRACT_ABI, TOKEN_CONTRACT_ADDRESS);
  //Read only so we can use the .call method.
  var balance = await contract.methods.contractBalance().call();
  log('balance = ' + balance);

  log(`ContractBalance shows ${Web3.utils.fromWei(balance)} ether`);
}

async function listPastEvents() {           //This function logs all events to the console.
  web3 = new Web3(Web3.givenProvider);
  const contract = new web3.eth.Contract(contractABI, await getContractAddress(ensName));
  contract.getPastEvents("allEvents", {
      fromBlock: 0,
      toBlock: 'latest'
    }) //, function(error, events){console.log(events);}) //Callback is optional.
    .then(function(events) {
      log('These are all smart contract events \n' + JSON.stringify(events));
    });
}

async function checkAccount() {
    web3 = new Web3(Web3.givenProvider); // provider from metamask
    web3.extend({ // web3.eth.requestAccounts() isn't available (yet)
        methods: [{
            name: 'eth_requestAccounts',
            call: 'eth_requestAccounts',
            params: 0
        }]
    });
    var result=await web3.eth_requestAccounts().catch(x=>log(x.message));
    log(`Account address = ${JSON.stringify(result)}`);

    ethereum.on('accountsChanged', newacts => {
        acts=newacts;
        log(`Metamask account changed to: ${JSON.stringify(acts)}`);
    });
}

async function checkChain() {
    web3 = new Web3(Web3.givenProvider); // provider from metamask
    var chainId=await web3.eth.getChainId();
    log(`We are on chain: ${JSON.stringify(chainId)}`);
    ethereum.on('chainChanged', newChain);
    ethereum.on('networkChanged',newChain); // still used in metamask mobile
//    ethereum.on('chainIdChanged',newChain);      // temp workaround
    ethereum.autoRefreshOnNetworkChange = false; // temp workaround
}

function newChain(newchainId) {
    chainId=newchainId;
    log(`New chain detected: ${chainId}`);
}

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

async function OnConnect(provider) {
    const web3 = new Web3(provider); // add provider to web3
    var acts=await web3.eth.getAccounts().catch(log);
    log(`Here are the accounts: ${JSON.stringify(acts)}`);
}

function f() {
    callApprove();
}

web3connector();
