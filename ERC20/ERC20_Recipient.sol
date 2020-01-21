 pragma solidity ^0.5.12;

import "https://github.com/OpenZeppelin/openzeppelin-contracts/blob/master/contracts/token/ERC20/ERC20.sol";
import "https://github.com/OpenZeppelin/openzeppelin-contracts/blob/master/contracts/token/ERC20/ERC20Detailed.sol";


contract Owner {
   address public owner;
   
   constructor() public {
      owner = msg.sender;
   }
   
   modifier onlyOwner {
      require(msg.sender == owner);
      _;
   }
   
}

contract Recipient is Owner {
    
    ERC20 private _token;
    
    uint32 public transactionCounter;
    
    event addresChanged(address _newAddress);
    event coinAdded(address _sender, uint32 counter);
    
    constructor (address token) public { 
        _token = ERC20(token); 
        transactionCounter = 0;
    }
    
    function setContractAddr(address _newTokenAddress) public onlyOwner {
        _token =ERC20(_newTokenAddress);
        emit addresChanged(_newTokenAddress);
    }
    
    function getContractTokenBalance() public view returns(uint256) {
        return _token.balanceOf(address(this));
    }
    
    function transferToOwner() public onlyOwner {
        _token.transfer(owner, getContractTokenBalance());
        //Transfers all tokens to the owner of the contract
    }
    
    function addGameCoin() public {
        _token.transferFrom(msg.sender, address(this), 1);
        transactionCounter++;
        emit coinAdded(msg.sender, transactionCounter);
    }
}
