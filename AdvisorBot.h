#pragma once

#include <string>
#include "OrderBook.h"

class AdvisorBot
{
  public:
    AdvisorBot();
    void init();
  private:
    // Task 1 commands:

    /** print welcome message */
    void welcomeMessage();
    /** gets user command for advisor bot */
    std::string getUserCmd();
    /** prints goodbye message */
    void exit();
    /** process user command for advisor bot*/
    void processUserCmd(std::string userCmd);
    /** lists all the available commands */
    void help();
    /** output help for the specificied command */
    void help(std::string cmd);
    /** lists available products */
    void prod();
    /** find minimum bid or ask for product in current time step */
    double min(std::string product, OrderBookType type, std::string timePeriod);
    /** prints min */
    void printMin(double min, std::string product, OrderBookType type);
    /** find minimum bid or ask for product in current time step */
    double max(std::string product, OrderBookType type, std::string timePeriod);
    /** prints min */
    void printMax(double max, std::string product, OrderBookType type);
    /** compute average ask or bid for the sent product over the sent number of time stemps*/
    double avg(std::string product, OrderBookType type, int timeSteps, std::string timePeriod);
    /** print avg */
    void printAvg(double avg,std::string product, OrderBookType type, int timeSteps, std::string timePeriod);
    /** predict max or min ask or bid for the sent product for the next time step using moving avg */
    double predict(std::string product, OrderBookType type,std::string typeMinMax, std::string timePeriod);
    /** prints predict */
    void printPredict(double predict, std::string product, std::string typeMinMax, OrderBookType type, std::string timePeriod);
    /** state current time in dataset, i.e. which timeframe are we looking at*/
    void time();
    /** move to next time step */
    void step();

    // Task 2 implement your own command :
    // analazyes the avg asks for current and previous time periods, for each products
    // compares them then prints what product is most actively droping and most raising
    // user can use that information to maybe quickly sell the droping product,
    // and buy the raising one
    // https://www.geeksforgeeks.org/returning-multiple-values-from-a-function-using-tuple-and-pair-in-c/ By: GeeksforGeeks, accessed 6/14/2022
    /** returns procantange of  product that is highest raising avg in asks, if returned value is 0, means there is no raisisng product */
    std::pair<std::string, double> mostActiveRaising (std::string timeStemp, std::string previousTimeStemp);
    /** returns procantange of  product that is most droping avg in asks, if returned value is 0, means there is no raisisng product*/
    std::pair<std::string, double> mostActiveDroping (std::string timeStemp, std::string previousTimeStemp);
    /** prints most acivaly droping and raising products */
    void printMostActive(std::pair<std::string, double> mostActiveRaising, std::pair<std::string, double> mostActiveDroping);


    // current time of the book
    std::string currentTime;
    // read the file and create data set orderBook
    OrderBook orderBook{"20200601.csv"}; // added some methods to use for advisor bot previousTimeStemp(), isProductInDataset(),
    // in order book entry added orderBookTypeToString
};
