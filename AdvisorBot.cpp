#include "AdvisorBot.h"
#include <iostream>
#include "OrderBookEntry.h"

AdvisorBot::AdvisorBot(){}

void AdvisorBot::init()
{
    // takes user command, processes it and answers,
    //while loop lets repeat the process unless user exits the aplication
    std::string userCmd;
    currentTime = orderBook.getEarliestTime();
    welcomeMessage();
    while(true)
    {
        userCmd = getUserCmd();
        if (userCmd == "exit") {exit(); break;} // print exit msg and exit the program
        processUserCmd(userCmd);
    }
}

void AdvisorBot::welcomeMessage()
{
    std::cout << std::endl;
    std::cout << "*--   ------   *- ------- -*   ------   --*" << std::endl ;
    std::cout << "*--   --   Welcome to Advisor Bot  --   --*" << std::endl ;
    std::cout << "*--   ------   *- ------- -*   ------   --*" << std::endl << std::endl ;
    help();
}

void AdvisorBot::exit()
{
    std::cout << "*--   ------   *- ------- -*   ------   --*" << std::endl ;
    std::cout << "*--   Thank you for using Advisor Bot   --*" << std::endl ;
    std::cout << "*--   ------   *- Goodbye -*   ------   --*" << std::endl << std::endl ;
}

std::string AdvisorBot::getUserCmd()
{
    // gets user input and returs it as single string
    std::string userCmd;
    std::cout << std::endl << "> ";
    std::getline(std::cin, userCmd);
    std::cout << std::endl;
    return userCmd;
}

void AdvisorBot::processUserCmd(std::string userCmd)
{
    // takes userCmd stirng, tokanisez it into a vector, than calls function based on command input,
    // this funcion is responsible for processing user input and feeding the function correct types and value parameters
    // if user wrote inncorect input, provides user with usefull information what informations are expected
    std::vector<std::string> tokens = CSVReader::tokenise(userCmd, ' ');
    if(tokens.empty()) // user just pressed enter https://www.geeksforgeeks.org/vectorempty-vectorsize-c-stl/ by AyushSaxena, webiste accesed on june/10/2022
    {
        std::cout << "Please type in the command." << std::endl;
        help(); // since user didn't type in command, show possible commands
        return; // https://stackoverflow.com/questions/346613/how-do-you-exit-from-a-void-function-in-c by mmx edited Nov 4, 2009 at 13:09
    }
    if (tokens[0] == "help") { // help or help <cmd>
        if (tokens.size() == 1) // if its just help
        {
            help();
        }
        if (tokens.size() == 2) // if its help <cmd> then tokens vector will have 2 string
        {
            help(tokens[1]);
        }
        if (tokens.size() > 2)  // when user typed help and than much more words
        // https://cplusplus.com/reference/vector/vector/size/ copyright: cplusplus.com, 2000-2022, webiste accesed on june/10/2022
        {
            std::cout << "Your help command is to long for help or help <cmd>." << std::endl;
            std::cout << "Try again..." << std::endl;
        }
    }
    else if (tokens[0] == "prod") { // prod
        if (tokens.size() > 1)
        {
            std::cout << "Prod command should not be followed by any words or numbers, try again" << std::endl;
            return;
        }
        prod();
    }
    else if (tokens[0] == "min") { // min product bid/ask
        // checking if user is writing 3 parameters
        if (tokens.size() != 3)
        {
            std::cout << "min command takes 3 words: min product bid/ask" << std::endl;
            return;
        }

        // testing if product is a known product ( exists in data set )
        if (!orderBook.isProductInDataset(tokens[1])) // if product not existitng
        {
            std::cout << "There is no such a product in our dataset." << std::endl;
            std::cout << "Try again, avaible products are:" << std::endl;
            prod(); // since user wrote wrong product show, what avaible products are
            return;
        }
        // testing last field that should be either bid ar ask
        if (!(tokens[2] == "bid" || tokens[2] == "ask"))
        {
            std::cout << "There are only two options: bid or ask. Try again." << std::endl;
            return;
        }
        printMin(
            min(tokens[1],OrderBookEntry::stringToOrderBookType(tokens[2]), currentTime), // calculate min for current time
            tokens[1], // tokens[1] is product
            OrderBookEntry::stringToOrderBookType(tokens[2]) // tokens[2] is orderbook type
            );
    }
    else if (tokens[0] == "max") { // max product bid/ask
        if (tokens.size() != 3) // checking if user is writing 3 parameters
        {
            std::cout << "max command takes 3 words: max product bid/ask" << std::endl;
            return;
        }
        // testing if product is a known product ( exists in data set )
        if (!orderBook.isProductInDataset(tokens[1])) // if product not existitng
        {
            std::cout << "There is no such a product in our dataset." << std::endl;
            std::cout << "Try again, avaible products are:" << std::endl;
            prod(); // since user wrote wrong product show, what avaible products are
            return;
        }
        // testing last field that should be either bid ar ask
        if (!(tokens[2] == "bid" || tokens[2] == "ask"))
        {
            std::cout << "There are only two options: bid or ask. Try again." << std::endl;
            return;
        }
        printMax(
            max(tokens[1],OrderBookEntry::stringToOrderBookType(tokens[2]), currentTime), // calculate max for current time
            tokens[1], // tokens[1] is product
            OrderBookEntry::stringToOrderBookType(tokens[2]) // tokens[2] is orderbook type
            );
    }
    else if (tokens[0] == "avg") { // avg product bid/ask timeSteps
        if (tokens.size() != 4) // checking if user is writing 4 parameters
        {
            std::cout << "avg command takes 4 parameters: avg product bid/ask time-steps" << std::endl;
            return;
        }
        // testing if product is a known product ( exists in data set )
        if (!orderBook.isProductInDataset(tokens[1])) // if product not existitng
        {
            std::cout << "There is no such a product in our dataset." << std::endl;
            std::cout << "Try again, avaible products are:" << std::endl;
            prod(); // since user wrote wrong product show, what avaible products are
            return;
        }
        // testing last field that should be either bid ar ask
        if (tokens[2] != "bid" && tokens[2] != "ask")
        {
            std::cout << "There are only two options: bid or ask. Try again." << std::endl;
            return;
        }
        // testing if last field is a number
        int timeSteps;
        try{
            timeSteps = std::stoi(tokens[3]);
        }
        catch(const std::exception& e) {
            std::cout << "Make sure that the fourth parameter is a number. Try again." << std::endl;
            return;
        }
        printAvg(
            avg(tokens[1], OrderBookEntry::stringToOrderBookType(tokens[2]), timeSteps, currentTime),
            tokens[1], // product
            OrderBookEntry::stringToOrderBookType(tokens[2]), // orderbook type
            timeSteps, // time steps as int
            currentTime // current time global val - where advisor bot is at, in data set
        );
    }
    else if (tokens[0] == "predict") { // predict min/max product bid/ask
        // checking if user is writing 4 parameters
        if (tokens.size() != 4)
        {
            std::cout << "predict command takes 4 parameters: predict min/max product bid/ask" << std::endl;
            return;
        }
        if (tokens[1] != "min" && tokens[1] != "max")
        {
            std::cout << "There are only two options: min or max. Try again." << std::endl;
            return;
        }
        // testing if product is a known product ( exists in data set )
        if (!orderBook.isProductInDataset(tokens[2])) // if product not existitng
        {
            std::cout << "There is no such a product in our dataset." << std::endl;
            std::cout << "Try again, avaible products are:" << std::endl;
            prod(); // since user wrote wrong product show, what avaible products are
            return;
        }
        // testing last field that should be either bid ar ask
        if (tokens[3] != "bid" && tokens[3] != "ask")
        {
            std::cout << "There are only two options: bid or ask. Try again." << std::endl;
            return;
        }
        // tokens[1] - min/max, tokens[2] - product, tokens[3] - bid/ask
        printPredict( //(predict, product, typeMinMax, type, timePeriod)
            predict(tokens[2], OrderBookEntry::stringToOrderBookType(tokens[3]),tokens[1], currentTime),//predict - (product, type,typeMinMax,timePeriod)
            tokens[2], // product
            tokens[1], // min/max
            OrderBookEntry::stringToOrderBookType(tokens[3]), // bid/ask
            currentTime // timePeriod print from current since that is where advisor bot is at the moment of call this function
        );
    }
    else if (tokens[0] == "time") { // time
        if (tokens.size() > 1)
        {
            std::cout << "time command should not be followed by any words or numbers, try again" << std::endl;
            return;
        }
        time();
    }
    else if (tokens[0] == "step") { // step
        if (tokens.size() > 1)
        {
            std::cout << "step command should not be followed by any words or numbers, try again" << std::endl;
            return;
        }
        step();
    }
    else if (tokens[0] == "most-active") { // most-active
        // if user writes more then 1 word
        if (tokens.size() > 1)
        {
            std::cout << "most-active command should not be followed by any words or numbers, try again" << std::endl;
            return;
        }
        if (orderBook.getPreviousTime(currentTime) == "") { // when previous time not found
            std::cout << "I don't have enough data for that command please do a time step and ask again." << std::endl;
            return;
        }
        printMostActive(
            mostActiveRaising(currentTime, orderBook.getPreviousTime(currentTime)),
            mostActiveDroping(currentTime, orderBook.getPreviousTime(currentTime))
        );
    }
    else { // random input - user command not recognized
        std::cout << "Command not recognized" << std::endl;
        help(); // prints avaible commands since user wrote inncorect command
    }
}

void AdvisorBot::help()
{
    std::cout<< "The available commands are:" << std::endl;
    std::cout <<  "help, help <cmd>, prod, min, max, avg, predict, time, step, most-active, exit"<< std::endl;
}

void AdvisorBot::help(std::string cmd)
{
    if (cmd == "help")
    {
        std::cout << "Lists all available commands" << std::endl;
    }
    else if (cmd == "help <cmd>") {
        std::cout << "Output help for the specific command." << std::endl;
        std::cout << " in place of <cmd> type in min, or time - one of available commands, to get help information for it" << std::endl;
    }
    else if (cmd == "prod") {
        std::cout << "Lists available products" << std::endl;
        std::cout << "prod - is a single word command" << std::endl;
    }
    else if (cmd == "min") {
        std::cout << "Find minimum bid or ask for product in current time step" << std::endl;
        std::cout << "min command takes 3 words: min product bid/ask" << std::endl;
    }
    else if (cmd == "max") {
        std::cout << "Find maximum bid or ask for product in current time step" << std::endl;
        std::cout << "max command takes 3 words: max product bid/ask" << std::endl;
    }
    else if (cmd == "avg") {
        std::cout << "Compute average ask or bid for the sent product over the sent number of time stemps" << std::endl;
        std::cout << "avg command takes 4 parameters: avg product bid/ask time-steps" << std::endl;
    }
    else if (cmd == "predict") {
        std::cout << "Predict max or min - ask or bid for the chosen product for the next time step" << std::endl;
        std::cout << "predict command takes 4 parameters: predict min/max product bid/ask" << std::endl;
    }
    else if (cmd == "time") {
        std::cout << "State current time in dataset, i.e. which timeframe are we looking at" << std::endl;
        std::cout << "time - is a single word command" << std::endl;
    }
    else if (cmd == "step") {
        std::cout << "Move to next time step" << std::endl;
        std::cout << "step - is a single word command" << std::endl;
    }
    else if (cmd == "most-active") {
        std::cout << "Provides information which product is the most raising and the most falling" << std::endl;
        std::cout << "most-active - is a single word command" << std::endl;
    }
    else if (cmd == "exit") {
        std::cout << "Exit the aplication" << std::endl;
        std::cout << "exit - is a single word command" << std::endl;
    }
    else {
        std::cout << "no such a command, make sure to type in command from avaible commands after help" << std::endl;
        help(); // since user wrote non existing command print the list of commands that advisor bot understands
    }

}

void AdvisorBot::prod()
{
    for (std::string const& p : orderBook.getKnownProducts()) // code taken from MerkelMain::printMarketStats()
    {
        std::cout << "Product: " << p << std::endl;
    }
}

double AdvisorBot::min(std::string product, OrderBookType type, std::string timePeriod)
{
    std::vector<OrderBookEntry> entries = orderBook.getOrders(type, product, timePeriod); // from MerkelMain::printMarketStats() - adjusted to need
    return OrderBook::getLowPrice(entries);
}

void AdvisorBot::printMin(double min, std::string product, OrderBookType type)
{
    std::cout << min << " is the min "
              << OrderBookEntry::orderBookTypeToString(type) << " for "
              << product << std::endl;

}

double AdvisorBot::max(std::string product, OrderBookType type, std::string timePeriod)
{
    std::vector<OrderBookEntry> entries = orderBook.getOrders(type, product, timePeriod); // from MerkelMain::printMarketStats() - adjusted to need
    return OrderBook::getHighPrice(entries);
}

void AdvisorBot::printMax(double max, std::string product, OrderBookType type)
{
    std::cout << max << " is the max "
              << OrderBookEntry::orderBookTypeToString(type) << " for "
              << product << std::endl;

}

double AdvisorBot::avg(std::string product, OrderBookType type, int timeSteps, std::string timePeriod)
{
    std::vector<OrderBookEntry> entries; // store entires for counting avg
    double avg = 0; // store avg value  in the begiging will store sum of avg over timeStemps then devidded by cardinality of how many avg we calculated
    int howManyTimeSteps = timeSteps + 1 ;// to devide sum of avg by
    do
    {
        entries = orderBook.getOrders(type, product, timePeriod);
        avg += orderBook.getAvgPrice(entries);
        timeSteps--;
        timePeriod = orderBook.getPreviousTime(timePeriod);
    }
    while (timeSteps >= 0 && timePeriod != "");

    // calculate avg
    avg = avg / (howManyTimeSteps - (timeSteps + 1)); // this is a little complicated so that the devidng number changes based on how many timesteps back we were able to go

    return avg;
}

void AdvisorBot::printAvg(double avg,std::string product, OrderBookType type, int timeSteps, std::string timePeriod)
{
    // logic for clear msg about how many time steps advisor bot was able to go back to calculate that avg,
    // because if user tapes avg from last 10 time steps, but bot only has data for last 5 time steps
    // then I think bot should comunicate that, other than just providing avg
    int howManyTimeSteps = timeSteps + 1 ;
    do
    {
        timeSteps--;
        timePeriod = orderBook.getPreviousTime(timePeriod);
    }
    while (timeSteps >= 0 && timePeriod != "");

    // if user ask for more last time stamps then data availabile
    if(timeSteps + 1 != 0)
        std::cout << "I don't have data for that far back. Analyzing as much as possible..." << std::endl;

    // printing the avarage for just current or more than currant
    std::cout << "The avg for " << product << " " << OrderBookEntry::orderBookTypeToString(type) << " price ";
    if (howManyTimeSteps == 1 || howManyTimeSteps == timeSteps + 2) // just the current time frame
    {
        std::cout << "in current time period ";
    }
    else {
        std::cout << "over the current and " << (howManyTimeSteps - (timeSteps + 2));
        // if 1 then "time step", if many then "time steps"
        if ((howManyTimeSteps - (timeSteps + 2)) == 1){std::cout << " time step back ";}
        else {std::cout<< " time steps back ";}
    }
    std::cout << "is : " << avg << std::endl;
}

double AdvisorBot::predict(std::string product, OrderBookType type,std::string typeMinMax, std::string timePeriod)
{
    // using moving avg, try to get up to 10 avg values back (if possible) to predict next value
    double predict= 0;
    int i; // also will be a divider
    for (i = 1; i<= 10; i++)
    {
        if (typeMinMax == "min") predict += min(product, type, timePeriod);
        if (typeMinMax == "max") predict += max(product, type, timePeriod);
        if (orderBook.getPreviousTime(timePeriod) == "") break;
        timePeriod = orderBook.getPreviousTime(timePeriod);
    }
    predict = predict / i;
    return predict;
}

void AdvisorBot::printPredict(double predict, std::string product, std::string typeMinMax, OrderBookType type, std::string timePeriod)
{
    int i; // to print how much data bot was able to analyze to base prediction of
    for (i = 1; i <= 10 ; i++){
        if (orderBook.getPreviousTime(timePeriod) == "") break;
        timePeriod = orderBook.getPreviousTime(timePeriod);
    }
    std::cout << "Based on " << typeMinMax << " " << OrderBookEntry::orderBookTypeToString(type) << " of " << product
              << " avg last " << i << " values. The next value could be: " << predict << std::endl;
}

void AdvisorBot::time()
{
    // used tokenise and change the parametr go have time clear with just seconds
    std::vector<std::string> tokens = CSVReader::tokenise(currentTime, '.');
    std::cout << "Current time stemp is: " << tokens[0] << std::endl;
}

void AdvisorBot::step()
{
    // note that advisor bot is just advisor, it reads and analyzes data, therefore does not invoke a matching engine on the time step
    std::cout << "Going to next time frame. " << std::endl;
    currentTime = orderBook.getNextTime(currentTime);
}

std::pair<std::string, double> AdvisorBot::mostActiveRaising(std::string timeStemp, std::string previousTimeStemp)
{
    double previousAvg, currentAvg, procChange;
    double procMostRaising = 0;
    std::string prodMostRaising;

    for (std::string const& p : orderBook.getKnownProducts()) // code taken from MerkelMain::printMarketStats()
    {
        previousAvg = avg(p, OrderBookType::ask, 0,previousTimeStemp);
        currentAvg = avg(p, OrderBookType::ask, 0,timeStemp);
        procChange = (100 * currentAvg / previousAvg) - 100; // if positive it raised if negative it droped
        if (procChange > procMostRaising) // finding the most procantege change in the avg ask values from timeStemp and previous to that time stemp
        {
            procMostRaising = procChange;
            prodMostRaising = p;
        }
    }
    return std::make_pair(prodMostRaising, procMostRaising);
}

std::pair<std::string, double> AdvisorBot::mostActiveDroping(std::string timeStemp, std::string previousTimeStemp)
{
    double previousAvg, currentAvg, procChange;
    double procMostDroping = 0;
    std::string prodMostDroping;

    for (std::string const& p : orderBook.getKnownProducts()) // code taken from MerkelMain::printMarketStats()
    {
        previousAvg = avg(p, OrderBookType::ask, 0,previousTimeStemp);
        currentAvg = avg(p, OrderBookType::ask, 0,timeStemp);
        procChange = (100 * currentAvg / previousAvg) - 100; // if positive it raised if negative it droped
        if (procChange < procMostDroping) // finding the most procantege change in the avg ask values from timeStemp and previous to that time stemp
        {
            procMostDroping = procChange;
            prodMostDroping = p;
        }
    }
    return std::make_pair(prodMostDroping, procMostDroping);
}

void AdvisorBot::printMostActive (std::pair<std::string, double> mostActiveRaising, std::pair<std::string, double> mostActiveDroping)
{
    // print what product is rasining and how many %
    if (mostActiveRaising.second == 0) { // if mostActiveRaising double value is 0 it means there is no product with positive change
        std::cout << "Sad none of the product is raisisng..." << std::endl;
    }
    else {
        std::cout << "Most actively raising is: " << mostActiveRaising.first << " at: " << mostActiveRaising.second << "% maybe time to buy?" << std::endl;
    }
    // print what product is droping and how many %
    if (mostActiveDroping.second == 0) { // if mostActiveDroping double value is 0 it means there is no product with negative change
        std::cout << "Awesome there is no product that is dropping in price." << std::endl;
    }
    else {
        std::cout << "Most actively droping is: " << mostActiveDroping.first << " at: " << mostActiveDroping.second << "% maybe time to sell?" << std::endl;
    }
}
