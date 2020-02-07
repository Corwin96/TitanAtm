/*jshint esversion: 8 */

//The amount of tokens you want transferred
const APPROVE_VALUE = 1;

//The address of the ERC20 token contract used
const TOKEN_CONTRACT_ADDRESS = '0x7E88BcaD9218b082A85c57C6bFDAD89087A0EC87';

//The ABI of the ERC20 token contract
const TOKEN_CONTRACT_ABI = [
  {
    "constant": false,
    "inputs": [
      {
        "internalType": "address",
    "name": "spender",
    "type": "address"
      },
      {
    "internalType": "uint256",
    "name": "amount",
    "type": "uint256"
      }
    ],
    "name": "approve",
    "outputs": [
      {
        "internalType": "bool",
    "name": "",
    "type": "bool"
      }
     ],
     "payable": false,
     "stateMutability": "nonpayable",
     "type": "function"
  }
];

//The address of the recipient contract
const ARCADE_CONTRACT_ADDRESS = '0x335a1f5634A440Ae4ca59adddd42AcB4E8dDbeF3';

//The ABI of the recipient contract
const ARCADE_CONTRACT_ABI = [
	{
		"inputs": [
			{
				"internalType": "address",
				"name": "token",
				"type": "address"
			}
		],
		"payable": false,
		"stateMutability": "nonpayable",
		"type": "constructor"
	},
	{
		"anonymous": false,
		"inputs": [
			{
				"indexed": false,
				"internalType": "address",
				"name": "_newAddress",
				"type": "address"
			}
		],
		"name": "addresChanged",
		"type": "event"
	},
	{
		"anonymous": false,
		"inputs": [
			{
				"indexed": false,
				"internalType": "address",
				"name": "_sender",
				"type": "address"
			}
		],
		"name": "coinAdded",
		"type": "event"
	},
	{
		"constant": false,
		"inputs": [],
		"name": "addGameCoin",
		"outputs": [],
		"payable": false,
		"stateMutability": "nonpayable",
		"type": "function"
	},
	{
		"constant": true,
		"inputs": [],
		"name": "getContractTokenBalance",
		"outputs": [
			{
				"internalType": "uint256",
				"name": "",
				"type": "uint256"
			}
		],
		"payable": false,
		"stateMutability": "view",
		"type": "function"
	},
	{
		"constant": false,
		"inputs": [
			{
				"internalType": "address",
				"name": "_newTokenAddress",
				"type": "address"
			}
		],
		"name": "setContractAddr",
		"outputs": [],
		"payable": false,
		"stateMutability": "nonpayable",
		"type": "function"
	},
	{
		"constant": true,
		"inputs": [],
		"name": "transactionCounter",
		"outputs": [
			{
				"internalType": "uint32",
				"name": "",
				"type": "uint32"
			}
		],
		"payable": false,
		"stateMutability": "view",
		"type": "function"
	},
	{
		"constant": false,
		"inputs": [],
		"name": "transferToOwner",
		"outputs": [],
		"payable": false,
		"stateMutability": "nonpayable",
		"type": "function"
	}
];
