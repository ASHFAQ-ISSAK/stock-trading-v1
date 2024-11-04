# Stock Trading System

## Overview
The Stock Trading System is a simple command-line application that simulates a stock trading environment. Users can manage stocks, buy and sell shares, and track their portfolio performance over time. The application maintains a record of transactions and displays market trends.

## Features
- Add stocks to the market with a symbol, current price, and available quantity.
- Update stock prices and track price history.
- Buy and sell stocks while maintaining a portfolio.
- Display portfolio details, transaction history, and market trends.
- Calculate overall portfolio statistics, including total value, profit/loss, and best/worst performing stocks.

## Structure

### Data Structures
- **Stock**: Represents a stock with attributes like symbol, current price, quantity available, and price history.
- **Portfolio**: Represents an entry for stocks purchased by the user.
- **Transaction**: Represents a buy or sell transaction with details about the stock, type, quantity, price, and timestamp.

### Global Variables
- `stocks`: An array to store available stocks.
- `portfolio`: An array to store the user's stock portfolio.
- `transactions`: An array to store all transactions made by the user.

## Functions
- **initializeSystem()**: Initializes the system and allocates memory for stock, portfolio, and transaction arrays.
- **cleanupSystem()**: Frees dynamically allocated memory.
- **addStock()**: Adds a new stock to the market.
- **updatePrice()**: Updates the price of a specific stock.
- **buyStock()**: Handles the purchase of stocks by the user.
- **sellStock()**: Handles the sale of stocks by the user.
- **displayPortfolio()**: Displays the current portfolio, including stock details and profit/loss.
- **displayTransactionHistory()**: Displays a history of all transactions.
- **displayMarketTrends()**: Displays current market trends, including price changes for stocks.
- **calculatePortfolioStats()**: Calculates and displays overall portfolio statistics.

## Usage
1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd stock-trading-system

2. Compile the code:
gcc -o stock_trading stock_trading_system.c

3.Run the program
./stock_trading
