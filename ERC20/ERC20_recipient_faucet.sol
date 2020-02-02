pragma solidity ^0.5.12;

import "https://github.com/OpenZeppelin/openzeppelin-contracts/blob/master/contracts/token/ERC20/ERC20.sol";
import "https://github.com/OpenZeppelin/openzeppelin-contracts/blob/master/contracts/token/ERC20/ERC20Detailed.sol";
import "https://github.com/OpenZeppelin/openzeppelin-contracts/blob/master/contracts/math/SafeMath.sol";
import "https://github.com/OpenZeppelin/openzeppelin-contracts/blob/master/contracts/ownership/Ownable.sol";

///@author Mathieu Gatsonides, Corwin van Dalen
///@notice Recipient contract for a chosen ERC20 token
///@dev Has a faucet function to withdraw a token for demo purposes
contract Recipient is Ownable {
    
    ERC20 public token;
    uint256 public transactionNum;
    
    mapping (address => bool) public tokenreceivers; //this mapping is only used for the demo to make sure an address can only withdraw once
        
    event addressChanged(address _newAddress);
    event coinAdded(address sender, uint256 transactionNum);
    event tokenSent(address receiver); //only used for the demo
    
    ///@dev constructor takes the token contract address as an argument
    constructor (address _token) public { 
        token = ERC20(_token); 
        transactionNum = 0;
    }
    
    ///@dev change the contract address if a new token will be used
    function setContractAddr(address _newTokenAddress) public onlyOwner {
        token = ERC20(_newTokenAddress);
        emit addressChanged(_newTokenAddress);
    }
    
    ///@notice used to check the current token balance of the contract
    function getContractTokenBalance() public view returns(uint256) {
        return token.balanceOf(address(this));
    }
    
    ///@notice withdraw all the tokens to the owners address
    function transferToOwner() public onlyOwner {
        token.transfer(owner, getContractTokenBalance());
        //Transfers all tokens to the owner of the contract
    }
    
    ///@notice used to add a coin in the game
    ///@dev requires approval before being able to be called, this is done in the javascript part of the site
    function addGameCoin() public {
        token.transferFrom(msg.sender, address(this), 1);
        transactionNum++;
        emit coinAdded(msg.sender, transactionNum);
    }
    
    ///@notice withdraw a token to the senders address, every address can only use this once
    ///@dev this is purely for demo purposes where visitors don't own any tokens
    function getToken() public {
        require(getContractTokenBalance() > 1);
        require(!(tokenreceivers[msg.sender]));
        token.transfer(msg.sender, 1);
        tokenreceivers[msg.sender] = true;
        emit tokenSent(msg.sender);
    }
}
