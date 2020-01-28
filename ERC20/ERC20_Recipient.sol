pragma solidity ^0.5.12;

import "https://github.com/OpenZeppelin/openzeppelin-contracts/blob/master/contracts/token/ERC20/ERC20.sol";
import "https://github.com/OpenZeppelin/openzeppelin-contracts/blob/master/contracts/token/ERC20/ERC20Detailed.sol";
import "https://github.com/OpenZeppelin/openzeppelin-contracts/blob/master/contracts/math/SafeMath.sol";


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
    
    ERC20 public token;
    uint256 public transactionNum;
        
    event addresChanged(address _newAddress);
    event coinAdded(address sender, uint256 transactionNum);
    
    constructor (address _token) public { 
        token = ERC20(_token); 
        transactionNum = 0;
    }
    
    function setContractAddr(address _newTokenAddress) public onlyOwner {
        token = ERC20(_newTokenAddress);
        emit addresChanged(_newTokenAddress);
    }
    
    
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
        transactionNum++;
        emit coinAdded(msg.sender, transactionNum);
    }
}


contract Token_erc20 is ERC20, ERC20Detailed {

    /**
     * @dev Constructor that gives _msgSender() all of existing tokens.
     */
    constructor () public ERC20Detailed("TestTokenEagleWorks", "TTEW", 18) {
        _mint(msg.sender, 10000 * (10 ** uint256(decimals())));
    }
}
