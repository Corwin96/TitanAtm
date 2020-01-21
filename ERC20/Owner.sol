pragma solidity ^0.5.12;

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
