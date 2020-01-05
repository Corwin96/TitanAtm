pragma solidity ^0.5.0;

import "https://github.com/OpenZeppelin/openzeppelin-contracts/blob/master/contracts/token/ERC777/IERC777.sol";
import "https://github.com/OpenZeppelin/openzeppelin-contracts/blob/master/contracts/introspection/IERC1820Registry.sol";
import "https://github.com/OpenZeppelin/openzeppelin-contracts/blob/master/contracts/token/ERC777/IERC777Recipient.sol";

/**
 * @title Simple777Recipient
 * @dev Very simple ERC777 Recipient
 */
contract Simple777Recipient is IERC777Recipient {

    IERC1820Registry private _erc1820 = IERC1820Registry(0x1820a4B7618BdE71Dce8cdc73aAB6C95905faD24);
    bytes32 constant private TOKENS_RECIPIENT_INTERFACE_HASH = keccak256("ERC777TokensRecipient");

    IERC777 private _token;

    event DoneStuff(address operator, address from, address to, uint256 amount, bytes userData, bytes operatorData);

    constructor (address token) public {
        _token = IERC777(token);

        _erc1820.setInterfaceImplementer(address(this), TOKENS_RECIPIENT_INTERFACE_HASH, address(this));
    }

    function tokensReceived(
        address operator,
        address from,
        address to,
        uint256 amount,
        bytes calldata userData,
        bytes calldata operatorData
    ) external {
        require(msg.sender == address(_token), "Simple777Recipient: Invalid token");

        // do stuff
        emit DoneStuff(operator, from, to, amount, userData, operatorData);
    }
}