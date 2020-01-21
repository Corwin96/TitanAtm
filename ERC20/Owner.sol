pragma solidity ^0.5.12;

contract Owner {
   address private _owner;
   
   constructor() public {
      _owner = msg.sender;
   }
   
   modifier onlyOwner {
      require(msg.sender == _owner);
      _;
   }
   
}
