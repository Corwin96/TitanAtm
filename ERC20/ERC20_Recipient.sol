pragma solidity ^0.5.12;

import "https://github.com/OpenZeppelin/openzeppelin-contracts/blob/master/contracts/token/ERC20/ERC20.sol";
import "https://github.com/OpenZeppelin/openzeppelin-contracts/blob/master/contracts/token/ERC20/ERC20Detailed.sol";
import "https://github.com/Corwin96/TitanAtm/blob/develop/ERC20/Owner.sol";

contract Recipient is Owner {
    
    ERC20 token = ERC20(0x883B6d3E97063858e72b82d030F7eA3B7556cb5A); //FILL IN WITH ERC20 ADDRESS
    
//    function setContractAddr(address addr) public onlyOwner {
//        ERC20 = addr;
        //ERC20 contract changed
//    }
    
    function getContractTokenBalance() public view returns(uint256) {
        return token.balanceOf(address(this));
    }
    
    function transferToOwner() public onlyOwner {
        token.transfer(owner, getContractTokenBalance());
        //Transfers all tokens to the owner of the contract
    }
    
    function addGameCoin() public {
       // token.approve(address(this), 0); THIS IS NEEDED BUT SHOULD BE IMPLEMENTED IN THE JAVASCRIPT. ALLOW THE CONTRACT TO USE YOUR TOKENS.
        token.transferFrom(msg.sender, address(this), 1);
    }
}


