pragma solidity ^0.5.12;

import "https://github.com/OpenZeppelin/openzeppelin-contracts/blob/master/contracts/token/ERC20/ERC20.sol";
import "https://github.com/OpenZeppelin/openzeppelin-contracts/blob/master/contracts/token/ERC20/ERC20Detailed.sol";
import "https://github.com/Corwin96/TitanAtm/blob/develop/ERC20/Owner.sol";

contract Recipient is Owner {
    
    ERC20 private _token;
    
    event addresChanged(address _newAddress);
    
    constructor (address token) public { 
        _token = ERC20(token); 
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
       // token.approve(address(this), 0); THIS IS NEEDED BUT SHOULD BE IMPLEMENTED IN THE JAVASCRIPT. ALLOW THE CONTRACT TO USE YOUR TOKENS.
        _token.transferFrom(msg.sender, address(this), 1);
    }
}
