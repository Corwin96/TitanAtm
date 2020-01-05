pragma solidity ^0.5.0;

import "https://github.com/OpenZeppelin/openzeppelin-contracts/blob/master/contracts/token/ERC777/IERC777.sol";
import "https://github.com/OpenZeppelin/openzeppelin-contracts/blob/master/contracts/introspection/IERC1820Registry.sol";
import "https://github.com/OpenZeppelin/openzeppelin-contracts/blob/master/contracts/introspection/ERC1820Implementer.sol";
import "https://github.com/OpenZeppelin/openzeppelin-contracts/blob/master/contracts/token/ERC777/IERC777Sender.sol";

contract Simple777Sender is IERC777Sender, ERC1820Implementer {

    bytes32 constant public TOKENS_SENDER_INTERFACE_HASH = keccak256("ERC777TokensSender");

    event DoneStuff(address operator, address from, address to, uint256 amount, bytes userData, bytes operatorData);

    function senderFor(address account) public {
        _registerInterfaceForAddress(TOKENS_SENDER_INTERFACE_HASH, account);
    }

    function tokensToSend(
        address operator,
        address from,
        address to,
        uint256 amount,
        bytes calldata userData,
        bytes calldata operatorData
    ) external {

        // do stuff
        emit DoneStuff(operator, from, to, amount, userData, operatorData);
    }
}