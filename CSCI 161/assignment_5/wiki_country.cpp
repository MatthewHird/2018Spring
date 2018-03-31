//------------------------------------------------------------------------------
// @file       wiki_country.cpp
// @author     Matthew Hird
// @date       April 1, 2018
//
// @brief      The main function for the program pex5.
//------------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <exception>
#include <cmath>
#include <sstream>
#include <algorithm>
#include "wiki_country.h"
#include "dictionary.h"
#include "country_data.h"
#include "my_exceptions.h"


WikiCountry::WikiCountry()
    : term(false)
    , wiki_save("wikiCountry.txt")
{
    command_keys.insert("list", ONE);
    command_keys.insert("show", TWO);
    command_keys.insert("remove", THREE);
    command_keys.insert("add", FOUR);
    command_keys.insert("update", FIVE);
    command_keys.insert("help", SIX);
    command_keys.insert("exit", SEVEN);
}


WikiCountry::~WikiCountry()
{}


void WikiCountry::run()
{
    std::cout << "\n~~~~ Welcome to WikiCountry ~~~~\n\n"; 
    load_wiki();
    menu();
}


void WikiCountry::menu()
{
    int sel;

    while (term == false)
    {
        std::cout 
        << "Please enter a command listed below:\n" 
        << "\n"
        << " list     List the name of all countries currently in the system\n"
        << " show     Show the full information of a country\n"
        << " remove   Remove a country and its entry from the system\n"
        << " add      Add a new country and entry to the system\n"
        << " update   Update any information about a country (except its name)\n"
        << " help     Shows list of commands\n"
        << " exit     Terminates the program\n"
        << "\n" 
        << "------------------------------------------------------------------\n";
        
        try
        {
             std::string command = get_string(false);
             std::transform(command.begin(), command.end(), command.begin(), tolower);
             sel = command_keys.lookup(command); 
      
        }
        catch (std::exception& e)
        {
            sel = ZERO;
        }

        switch (sel)
        {                   
            case 1: // list
                list();
                break;
                
            case 2: // show
                show();
                break;
                
            case 3: // remove
                remove();
                break;
                
            case 4: // add
                add();
                break;
                
            case 5: // update
                update();
                break;

            case 6: // help
                std::cout << "help!\n";
                break;

            case 7: // exit
                terminate();
                break;
            
            default:
                std::cout << "Invalid entry: Please try again\n\n";
                break;
        }
    }
}


void WikiCountry::list()
{
    std::cout << "\n" << countries.get_key_list() << "\n";
}


void WikiCountry::show()
{
    std::cout << "Please enter the name of the country whose information you would like to see\n";
    std::string c_name = get_string(true);
    
    if (countries.has_key(c_name))
    {
        const CountryData& data = countries.lookup(c_name);
        
        std::cout << "Name: " << c_name << "\n"; 

        if (data.get_capital() != "")
        {
            std::cout << "Capital: " << data.get_capital() << "\n";
        }

        if (data.get_language() != "")
        {
            std::cout << "Languages: " << data.get_language() << "\n";
        }

        if (data.get_area() != -1)
        {
            std::cout << "Area: " << data.get_area() << "square kilometers\n";
        }
         if (data.get_population() != -1)
        {
            std::cout << "Population: " << data.get_population() << "\n";
        }

        if (data.get_description() != "")
        {
            std::cout << "Description: " << data.get_description() << "\n";
        }
    }
    else
    {
        std::cout << "Invalid entry\n";
    }

    std::cout << "\n";
}


void WikiCountry::remove()
{
    std::cout << "Please enter the name of the country whose information you would like to remove\n";
    std::string c_name = get_string(true);

    if (countries.has_key(c_name) == false)
    {
        std::cout << "Country was not in system\n";
        return;
    }

    const CountryData* data = &(countries.remove(c_name));
    delete data;
}


void WikiCountry::add()
{
    std::cout << "Please enter the name of the country you would like to add to the system\n";
    std::string c_name = get_string(false);
    
    if (countries.has_key(c_name))
    {
        std::cout << "Country already in system\n";
        return;
    }

    CountryData& data = *(new CountryData(c_name));
    
    std::cout << "\n";
    data.set_capital(get_string(true));
    
    std::cout << "\n";
    data.set_language(get_string(true));
    
    std::cout << "\n";
    double area = get_double();
    while (area == -2)
    {
        std::cout << "\n";
        area = get_double();
    }
    data.set_area(area);

    std::cout << "\n";
    long int pop = get_long_int();
    while (pop == -2)
    {
        std::cout << "\n";
        pop = get_long_int();
    }
    data.set_population(pop);

    std::cout << "\n";
    data.set_description(get_string(true));

    try
    {
        countries.insert(c_name, data);
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << "\n";
    }
}


void WikiCountry::update()
{
    std::cout << "Please enter the name of the country whose information you would like to remove\n";
    std::string c_name = get_string(true);

    if (countries.has_key(c_name) == false)
    {
        std::cout << "Country was not in system\n";
        return;
    }

    CountryData& new_data = *(new CountryData(countries.lookup(c_name)));
    
    std::cout << "Would you like to update the country's name field?\n";
    if (get_confirmation())
    {
        new_data.set_name(get_string(false));
    }

    std::cout << "Would you like to update the country's capital field?\n";
    if (get_confirmation())
    {
        new_data.set_capital(get_string(true));
    }

    std::cout << "Would you like to update the country's language field?\n";
    if (get_confirmation())
    {
        new_data.set_language(get_string(true));
    }

    std::cout << "Would you like to update the country's area field?\n";
    if (get_confirmation())
    {
        new_data.set_area(get_double());
    }

    std::cout << "Would you like to update the country's population field?\n";
    if (get_confirmation())
    {
        new_data.set_population(get_long_int());
    }

    std::cout << "Would you like to update the country's description field?\n";
    if (get_confirmation())
    {
        new_data.set_description(get_string(true));
    }

    try{
        const CountryData* old_data = &(countries.remove(c_name));
        delete old_data;
        countries.insert(new_data.get_name(), new_data);
    }
    catch (std::exception& e)
    {

    }
}


void WikiCountry::terminate()
{
    save_wiki();
    term = true;
}


int WikiCountry::load_wiki()
{
    int entry_count;
    std::ifstream fin;

    // fin.open("test_save.txt");
    fin.open(wiki_save.c_str());

    if (fin.fail()) 
    {
        return 0;
    }

    fin >> entry_count;
    fin.ignore();

    for (int i = 0; i < entry_count; i++) 
    {
        CountryData& data = *(new CountryData);
        fin >> data;
        std::string key = data.get_name();

        try
        {
            countries.insert(key, data);
        }
        catch(const std::exception& e)
        {
            std::cout << e.what() << "\n";
        }
    }
    fin.close();

    return entry_count;
}


int WikiCountry::save_wiki()
{
    int entry_count = countries.get_size();
    std::ofstream fout;
    std::stringstream sstr;
    sstr << countries.get_key_list();
    
    fout.open("test_save.txt");
    fout << entry_count << "\n";

    for (int i = 0; i < entry_count; i++ )
    {
        std::string c_name;
        getline(sstr, c_name);
        fout << countries.lookup(c_name);
    }

    fout.close();
    return entry_count;
}


bool WikiCountry::get_confirmation()
{
    char sel;
    
    do
    {
        std::cout << "Yes (Y) or No (N)\n"
                  << ">>>  ";
        std::cin.get(sel);                   
    
        if (sel != '\n') 
        {
            std::cin.ignore();
        }
        sel = toupper(sel);
    } while (sel != 'Y' && sel != 'N');
    
    if (sel == 'Y')
    {
        return true;
    }
    if (sel == 'N')
    {
        return false;
    }
}


long int WikiCountry::get_long_int()
{
    std::string str_form;
    long int int_form = 0;
    
    std::getline(std::cin, str_form);
    
    if (str_form.length() == 0) 
    {
        return -1;
    }
    
    for (int i = 0; i < str_form.length(); i++)
    {
        if (str_form[i] > '9' || str_form[i] < '0')
        {
            return -2;
        }
        int_form = int_form * 10;
        int_form += (str_form[i] - '0');
    }

    return int_form;
}


double WikiCountry::get_double()
{
    std::string str_form;
    double dbl_form = 0;
    int dec_point = false;
    int start_count = false;
    int count = 0;
    
    std::getline(std::cin, str_form);
    
    if (str_form.length() == 0) 
    {
        return -1;
    }

    for (int i = 0; i < str_form.length(); i++)
    {
        if (str_form[i] == '.')
        {
            if (dec_point == false)
            {
                dec_point = true;
                start_count = true;
            }
            else
            {
                return -2;
            }
        }
        else
        {
            if (str_form[i] > '9' || str_form[i] < '0')
            {
                return -2;
            }
            dbl_form = dbl_form * 10;
            dbl_form += (str_form[i] - '0');
            if (start_count == true)
            {
                count++;
            }
        }
    }

    dbl_form = dbl_form / (pow(10, count));
    return dbl_form;
}


std::string WikiCountry::get_string(bool blank_ok)
{
    int bad_input;
    std::string str_in;

    do 
    {
        bad_input = false;
        
        std::cout << ">>>  ";
        std::getline(std::cin, str_in);
        
        if (blank_ok == false && str_in.length() == 0) 
        {
            bad_input = true;
            std::cout << "Input must not be left blank\n";
        }
        else if (str_in.length() > 255)
        {
            bad_input = true;
            std::cout << "Input must be 255 characters or less\n";
        }
    } while (bad_input == true);

    return str_in;
}