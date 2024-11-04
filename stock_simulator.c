#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SYMBOL 10
#define MAX_STOCKS 100
#define MAX_TRANSACTIONS 1000

// Structure definitions
// Represents a stock with attributes like symbol, price, quantity, and price history

typedef struct {
    char symbol[MAX_SYMBOL];
    double currentPrice;
    int quantityAvailable;
    double priceHistory[30];  // Store last 30 days
    int historyCount;
} Stock;

// Represents a portfolio entry for a stock that a user has bought
typedef struct {
    char symbol[MAX_SYMBOL];
    int quantity;
    double buyPrice;
} Portfolio;

// Represents a single buy or sell transaction
typedef struct {
    char symbol[MAX_SYMBOL];
    char type;  // 'B' for buy, 'S' for sell
    int quantity;
    double price;
    time_t timestamp;
} Transaction;

// Global variables
// Dynamic arrays for storing stocks, portfolio items, and transactions

Stock *stocks = NULL;             //Array of Stock structures
int stockCount = 0;
Portfolio *portfolio = NULL;      //Array of Portfolio structures
int portfolioCount = 0;
Transaction *transactions = NULL; //Array of Transaction structures
int transactionCount = 0;

// Function prototypes
void initializeSystem();
void cleanupSystem();
void addStock(const char *symbol, double price, int quantity);
void updatePrice(const char *symbol, double newPrice);
int buyStock(const char *symbol, int quantity);
int sellStock(const char *symbol, int quantity);
void displayPortfolio();
void displayTransactionHistory();
void displayMarketTrends();
void calculatePortfolioStats();
Stock* findStock(const char *symbol);
Portfolio* findPortfolioItem(const char *symbol);

// Initialize the system
void initializeSystem() {
    // Allocate memory for the stock, portfolio, and transaction arrays
    stocks = (Stock*)malloc(MAX_STOCKS * sizeof(Stock));
    portfolio = (Portfolio*)malloc(MAX_STOCKS * sizeof(Portfolio));
    transactions = (Transaction*)malloc(MAX_TRANSACTIONS * sizeof(Transaction));
    
    // Check if the memory allocation was successful
    if (!stocks || !portfolio || !transactions) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    
    // Initialize the counts to 0
    stockCount = 0;
    portfolioCount = 0;
    transactionCount = 0;
}

// Cleanup system resources
void cleanupSystem() {
    // Free the dynamically allocated memory
    free(stocks);
    free(portfolio);
    free(transactions);
}

// Add a new stock to the system
void addStock(const char *symbol, double price, int quantity) {
    // Check if the maximum stock limit has been reached
    if (stockCount >= MAX_STOCKS) {
        printf("Maximum stocks limit reached!\n");
        return;
    }
    
    // Get a pointer to the next available stock
    Stock *stock = &stocks[stockCount++];
    
    // Copy the symbol, set the price and quantity, and initialize the price history
    strncpy(stock->symbol, symbol, MAX_SYMBOL - 1);
    stock->symbol[MAX_SYMBOL - 1] = '\0';
    stock->currentPrice = price;
    stock->quantityAvailable = quantity;
    stock->historyCount = 0;
    stock->priceHistory[stock->historyCount++] = price;
}

// Find a stock by symbol
Stock* findStock(const char *symbol) {
    // Linear search to find the stock with the given symbol
    for (int i = 0; i < stockCount; i++) {
        if (strcmp(stocks[i].symbol, symbol) == 0) {
            return &stocks[i];
        }
    }
    return NULL;
}

// Find a portfolio item by symbol
Portfolio* findPortfolioItem(const char *symbol) {
    // Linear search to find the portfolio item with the given symbol
    for (int i = 0; i < portfolioCount; i++) {
        if (strcmp(portfolio[i].symbol, symbol) == 0) {
            return &portfolio[i];
        }
    }
    return NULL;
}

// Update stock price
void updatePrice(const char *symbol, double newPrice) {
    // Find the stock with the given symbol
    Stock *stock = findStock(symbol);
    if (!stock) {
        printf("Stock not found!\n");
        return;
    }
    
    // Update the current price and shift the price history array
    stock->currentPrice = newPrice;
    if (stock->historyCount < 30) {
        stock->priceHistory[stock->historyCount++] = newPrice;
    } else {
        // Shift history array left and add new price
        for (int i = 0; i < 29; i++) {
            stock->priceHistory[i] = stock->priceHistory[i + 1];
        }
        stock->priceHistory[29] = newPrice;
    }
}

// Buy stock
int buyStock(const char *symbol, int quantity) {
    // Find the stock with the given symbol
    Stock *stock = findStock(symbol);
    if (!stock) {
        printf("Stock not found!\n");
        return 0;
    }
    
    // Check if the requested quantity is available
    if (stock->quantityAvailable < quantity) {
        printf("Insufficient quantity available!\n");
        return 0;
    }
    
    // Find the portfolio item with the given symbol
    Portfolio *item = findPortfolioItem(symbol);
    if (!item) {
        // If the item is not found, create a new one
        if (portfolioCount >= MAX_STOCKS) {
            printf("Portfolio is full!\n");
            return 0;
        }
        item = &portfolio[portfolioCount++];
        strncpy(item->symbol, symbol, MAX_SYMBOL - 1);
        item->symbol[MAX_SYMBOL - 1] = '\0';
        item->quantity = 0;
        item->buyPrice = 0;
    }
    
    // Update the portfolio item
    double totalCost = item->buyPrice * item->quantity;
    totalCost += stock->currentPrice * quantity;
    item->quantity += quantity;
    item->buyPrice = totalCost / item->quantity;
    
    // Update the stock quantity
    stock->quantityAvailable -= quantity;
    
    // Record the transaction
    if (transactionCount < MAX_TRANSACTIONS) {
        Transaction *trans = &transactions[transactionCount++];
        strncpy(trans->symbol, symbol, MAX_SYMBOL - 1);
        trans->symbol[MAX_SYMBOL - 1] = '\0';
        trans->type = 'B';
        trans->quantity = quantity;
        trans->price = stock->currentPrice;
        trans->timestamp = time(NULL);
    }
    
    return 1;
}

// Sell stock
int sellStock(const char *symbol, int quantity) {
    // Find the portfolio item with the given symbol
    Portfolio *item = findPortfolioItem(symbol);
    if (!item || item->quantity < quantity) {
        printf("Insufficient stocks in portfolio!\n");
        return 0;
    }
    
    // Find the stock with the given symbol
    Stock *stock = findStock(symbol);
    if (!stock) {
        printf("Stock not found!\n");
        return 0;
    }
    
    // Update the portfolio item
    item->quantity -= quantity;
    if (item->quantity == 0) {
        // Remove the item from the portfolio
        *item = portfolio[--portfolioCount];
    }
    
    // Update the stock quantity
    stock->quantityAvailable += quantity;
    
    // Record the transaction
    if (transactionCount < MAX_TRANSACTIONS) {
        Transaction *trans = &transactions[transactionCount++];
        strncpy(trans->symbol, symbol, MAX_SYMBOL - 1);
        trans->symbol[MAX_SYMBOL - 1] = '\0';
        trans->type = 'S';
        trans->quantity = quantity;
        trans->price = stock->currentPrice;
        trans->timestamp = time(NULL);
    }
    
    return 1;
}

// Display portfolio
void displayPortfolio() {
    printf("\nCurrent Portfolio:\n");
    printf("Symbol\tQuantity\tBuy Price\tCurrent Price\tProfit/Loss\n");
    printf("--------------------------------------------------------------\n");
    
    double totalValue = 0;
    double totalProfitLoss = 0;
    
    for (int i = 0; i < portfolioCount; i++) {
        Stock *stock = findStock(portfolio[i].symbol);
        if (!stock) continue;
        
        double currentValue = stock->currentPrice * portfolio[i].quantity;
        double buyValue = portfolio[i].buyPrice * portfolio[i].quantity;
        double profitLoss = currentValue - buyValue;
        
        printf("%s\t%d\t\t%.2f\t\t%.2f\t\t%.2f\n",
               portfolio[i].symbol,
               portfolio[i].quantity,
               portfolio[i].buyPrice,
               stock->currentPrice,
               profitLoss);
               
        totalValue += currentValue;
        totalProfitLoss += profitLoss;
    }
    
    printf("--------------------------------------------------------------\n");
    printf("Total Portfolio Value: %.2f\n", totalValue);
    printf("Total Profit/Loss: %.2f\n", totalProfitLoss);
}

// Display transaction history
void displayTransactionHistory() {
    printf("\nTransaction History:\n");
    printf("Time\t\t\tSymbol\tType\tQuantity\tPrice\n");
    printf("--------------------------------------------------------------\n");
    
    for (int i = 0; i < transactionCount; i++) {
        char *timeStr = ctime(&transactions[i].timestamp);
        timeStr[strlen(timeStr) - 1] = '\0';  // Remove newline
        
        printf("%s\t%s\t%c\t%d\t\t%.2f\n",
               timeStr,
               transactions[i].symbol,
               transactions[i].type,
               transactions[i].quantity,
               transactions[i].price);
    }
}

// Display market trends
void displayMarketTrends() {
    printf("\nMarket Trends:\n");
    printf("Symbol\tCurrent Price\tChange\t\t30-day Trend\n");
    printf("--------------------------------------------------------------\n");
    
    for (int i = 0; i < stockCount; i++) {
        double change = 0;
        if (stocks[i].historyCount > 1) {
            change = stocks[i].currentPrice - stocks[i].priceHistory[0];
        }
        
        printf("%s\t%.2f\t\t%.2f\t\t",
               stocks[i].symbol,
               stocks[i].currentPrice,
               change);
               
        // Using ASCII characters instead of Unicode arrows
        for (int j = 0; j < stocks[i].historyCount; j++) {
            if (j > 0) {
                if (stocks[i].priceHistory[j] > stocks[i].priceHistory[j-1])
                    printf("+");  // Up trend
                else if (stocks[i].priceHistory[j] < stocks[i].priceHistory[j-1])
                    printf("-");  // Down trend
                else
                    printf("=");  // No change
            }
        }
        printf("\n");
    }
}

// Calculate portfolio statistics
void calculatePortfolioStats() {
    if (portfolioCount == 0) {
        printf("Portfolio is empty!\n");
        return;
    }
    
    double totalValue = 0;
    double totalCost = 0;
    double highestValue = 0;
    double lowestValue = -1;
    char bestStock[MAX_SYMBOL];
    char worstStock[MAX_SYMBOL];
    
    for (int i = 0; i < portfolioCount; i++) {
        Stock *stock = findStock(portfolio[i].symbol);
        if (!stock) continue;
        
        double currentValue = stock->currentPrice * portfolio[i].quantity;
        double cost = portfolio[i].buyPrice * portfolio[i].quantity;
        double profitLoss = currentValue - cost;
        
        totalValue += currentValue;
        totalCost += cost;
        
        if (profitLoss > highestValue) {
            highestValue = profitLoss;
            strncpy(bestStock, portfolio[i].symbol, MAX_SYMBOL);
        }
        
        if (lowestValue == -1 || profitLoss < lowestValue) {
            lowestValue = profitLoss;
            strncpy(worstStock, portfolio[i].symbol, MAX_SYMBOL);
        }
    }
    
    printf("\nPortfolio Statistics:\n");
    printf("--------------------------------------------------------------\n");
    printf("Total Portfolio Value: %.2f\n", totalValue);
    printf("Total Cost Basis: %.2f\n", totalCost);
    printf("Overall Profit/Loss: %.2f\n", totalValue - totalCost);
    printf("Best Performing Stock: %s (%.2f)\n", bestStock, highestValue);
    printf("Worst Performing Stock: %s (%.2f)\n", worstStock, lowestValue);
    printf("Portfolio Diversity: %d stocks\n", portfolioCount);
}

// Main function with sample usage
int main() {
    // Initialize the system
    initializeSystem();
    
   // Add sample Kenyan stocks with realistic prices (in KES)
addStock("SCOM", 25.50, 10000);    // Safaricom PLC
addStock("EQTY", 45.75, 8000);     // Equity Group Holdings
addStock("KCB", 38.20, 7500);      // KCB Group
addStock("EABL", 155.25, 5000);    // East African Breweries
addStock("BAMB", 32.80, 6000);     // Bamburi Cement
addStock("COOP", 12.45, 12000);    // Cooperative Bank
addStock("KNGEN", 5.85, 15000);    // KenGen
addStock("BAT", 428.00, 3000);     // British American Tobacco Kenya
addStock("ABSA", 11.90, 9000);     // ABSA Bank Kenya
addStock("JUB", 289.75, 4000);     // Jubilee Holdings

// Simulate diverse trading scenarios
// Buy stocks
buyStock("SCOM", 1000);    // Initial investment in Safaricom
buyStock("EQTY", 500);     // Initial investment in Equity
buyStock("EABL", 200);     // Initial investment in EABL
buyStock("KNGEN", 2000);   // Initial investment in KenGen

// Simulate price changes (some up, some down)
updatePrice("SCOM", 27.80);     // Safaricom price increase (+9.0%)
updatePrice("EQTY", 42.30);     // Equity price decrease (-7.5%)
updatePrice("EABL", 168.50);    // EABL price increase (+8.5%)
updatePrice("KNGEN", 5.20);     // KenGen price decrease (-11.1%)

// More trading based on price movements
buyStock("EQTY", 300);      // Buy more on dip
sellStock("SCOM", 400);     // Take some profit
buyStock("KNGEN", 1000);    // Average down
sellStock("EABL", 50);      // Partial profit taking

// Additional price updates to show volatility
updatePrice("SCOM", 28.15);     // Further increase
updatePrice("EQTY", 43.90);     // Recovery
updatePrice("EABL", 162.75);    // Slight pullback
updatePrice("KNGEN", 5.45);     // Partial recovery
    
    // Display information
    displayPortfolio();
    displayTransactionHistory();
    displayMarketTrends();
    calculatePortfolioStats();
    
    // Cleanup
    cleanupSystem();
    
    return 0;
}
