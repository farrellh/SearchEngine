#include "webpage.h"
#include "searchengine.h"
using namespace std;

SearchEngine::SearchEngine()
{

}

SearchEngine::SearchEngine(ifstream &input, ifstream &query, ofstream &output)
{

	// Creating all of the WebPage objects and inserting into map
    while(getline(input, filename))
    {
    	WebPage * page = new WebPage(filename);
    	webpages.insert({filename, page});
    }

    IncomingSetter();
    WordMap();
}

SearchEngine::~SearchEngine()
{
	for (auto pages : webpages)
	{
		delete pages.second;
	}
}

void SearchEngine::IncomingSetter()
{
	// Loop through all webpages
    for (std::pair<std::string, WebPage*> element : webpages)
  	{
  		std::set<std::string> outgoing = element.second -> getOutLinks();

  		std::set<std::string>::iterator it; 

  		// Loop through outgoing links
		for(it = outgoing.begin(); it != outgoing.end(); it++) 
		{
			// If the outgoing link equals a filename in the WebPages map, update incoming set
			if(webpages.find(*it) != webpages.end())
			{
				webpages.find(*it)->second ->setInLinks(element.first);
			}
		}
  	}
}

void SearchEngine::WordMap()
{
	// Loop through all webpages
    for (std::pair<std::string, WebPage*> element : webpages)
  	{
  		std::set<std::string> words = element.second -> getWords();

  		std::set<std::string>::iterator it; 

  		// Loop through the set of words for particular WebPage
		for(it = words.begin(); it != words.end(); it++) 
		{
			// If the word is already in the word map, add the WebPage to the set
			if(wordmap.find(*it) != wordmap.end())
			{
				set<WebPage*>& s_ref = wordmap[(*it)];
    			s_ref.insert(element.second);
			}
			// If the word is not in the word map, create the map with the word and WebPage set
			else
			{
				set<WebPage*> temp;
				temp.insert(element.second);
				wordmap.insert(make_pair((*it), temp));
			}
		}
  	}
}

void SearchEngine::Query(std::ifstream &query, std::ofstream &output)
{
	while(getline(query, querytext))
    {
    	std::string querycommand;
    	stringstream ss;
    	ss << querytext;

    	ss >> querycommand;

    	// AND QUERY
    	if(querycommand == "AND")
    	{
    		if(ss.eof())
    		{
    			SingleWordSearch(querycommand, output);
    			continue;
    		}

   			string intersectword;
   			set<string> searchwords;
   			vector<string> intersectstring;
   			set<string> finalintersection;
   			map<string, int> intersectmap;

    		while(ss >> intersectword)
    		{
	    		searchwords.insert(intersectword);
    			Lowercase(intersectword);

    			if(wordmap.find(intersectword) != wordmap.end())
    			{
    				// Create the set of webpages matching to the word
					set<WebPage*> pagetoword = wordmap[intersectword];
					std::set<WebPage*>::iterator it; 

					// Loop through the set of WebPages links and insert the name into intersect set
					for(it = pagetoword.begin(); it != pagetoword.end(); it++) 
					{
						intersectstring.push_back((*it)->getName()); 
					}
    			}
    		}

    		// Iterate through the vector list of textfile names 
    		vector<string>::iterator it; 
    		for(it = intersectstring.begin(); it != intersectstring.end(); it++) 
			{
				// If the textfile name is already in the word map, update the int
				if(intersectmap.find(*it) != intersectmap.end())
				{
					intersectmap[(*it)] = intersectmap[(*it)]++;
				}
				// If the word is not in the word map, create the map with the word and int
				else
				{
					intersectmap[(*it)] = 1;
				}
			}

			// Loop through all the textfile names
		    for (std::pair<std::string, int> element : intersectmap)
		  	{
		  		// If the integer matches the number of words being searched for
		  		// insert the textfile name into the finalintersection set
		  		if(element.second == (signed int)searchwords.size())
		  		{
		  			finalintersection.insert(element.first);
		  		}
		  	}

		  	output << finalintersection.size() << "\n";

		  	// Loop through the final intersection
    		for(auto intersections : finalintersection) 
			{
				output << intersections << "\n";
			}
    	}

    	// OR QUERY
    	else if(querycommand == "OR")
    	{
    		if(ss.eof())
    		{
    			Lowercase(querycommand);
    			SingleWordSearch(querycommand, output);
    			continue;
    		}

    		std::string unionword;
   			set<std::string> unionset;

    		while(ss >> unionword)
    		{
    			Lowercase(unionword);
    			if(wordmap.find(unionword) != wordmap.end())
    			{
    				// Create the set of webpages matching to the word
					set<WebPage*> pagetoword = wordmap[unionword];
					std::set<WebPage*>::iterator it; 

					// Loop through the set of WebPages links and insert the name into union set
					for(it = pagetoword.begin(); it != pagetoword.end(); it++) 
					{
						unionset.insert((*it)->getName()); 
					}
    			}
    		}

    		output << unionset.size() << endl;
    		std::set<std::string>::iterator it; 

			// Loop through the set of WebPages links
			for(it = unionset.begin(); it != unionset.end(); it++) 
			{
				output << *it << endl; 
				
			}
    	}

    	// PRINT QUERY
    	else if(querycommand == "PRINT")
    	{
    		if(ss.eof())
    		{
    			Lowercase(querycommand);
    			SingleWordSearch(querycommand, output);
    			continue;
    		}
    		std::string page;
    		ss >> page;

    		if(webpages.find(page) != webpages.end())
    		{

	    		ifstream printfile(page);
	    		char character;
	    		while(printfile.get(character))
	    		{
	    			if(character == '(')
	    			{
	    				while(printfile.get(character))
	    				{
	    					if(character == ')')
	    					{
	    						break;
	    					}
	    				}
	    			}
	    			else
	    			{
	    				output << character;
	    			}
	    		}

	    		output << "\n";
    		}
    		else
    		{
    			output << "Invalid query\n";
    		}
    	}

    	// INCOMING QUERY
    	else if(querycommand == "INCOMING")
    	{
    		if(ss.eof())
    		{
    			Lowercase(querycommand);
    			SingleWordSearch(querycommand, output);
    			continue;
    		}

    		std::string page;
    		ss >> page;

    		if(webpages.find(page) != webpages.end())
    		{
	    		std::set<std::string> incoming = webpages.find(page)->second->getInLinks();
	    		output << incoming.size() << "\n";

	    		std::set<std::string>::iterator it; 

		  		// Loop through incoming links
				for(it = incoming.begin(); it != incoming.end(); it++) 
				{
					output << (*it) << "\n";
				}
			}
			else
    		{
    			output << "Invalid query\n";
    		}
    	}

    	// OUTGOING QUERY
    	else if(querycommand == "OUTGOING")
    	{
    		if(ss.eof())
    		{
    			Lowercase(querycommand);
    			SingleWordSearch(querycommand, output);
    			continue;
    		}

    		std::string page;
    		ss >> page;

    		if(webpages.find(page) != webpages.end())
    		{
	    		std::set<std::string> outgoing = webpages.find(page)->second->getOutLinks();
	    		output << outgoing.size() << "\n";

	    		std::set<std::string>::iterator it; 

		  		// Loop through outgoing links
				for(it = outgoing.begin(); it != outgoing.end(); it++) 
				{
					output << (*it) << "\n";
				}
			}
			else
    		{
    			output << "Invalid query\n";
    		}
    	}

    	// ONE WORD QUERY
    	else
    	{
    		std::string extraword;
    		if(ss >> extraword)
    		{
    			output << "Invalid query\n";
    		}

    		else
    		{
    			SingleWordSearch(querycommand, output);
    		}
    	}
    }

    return;

}

void SearchEngine::SingleWordSearch(std::string querycommand, ofstream &output)
{
	// If the query is in the word bank
	if(wordmap.find(querycommand) != wordmap.end())
	{
		// Create the set of webpages matching to the word
		set<WebPage*> pagetoword = wordmap[querycommand];
		std::set<WebPage*>::iterator it; 
		output << pagetoword.size() << "\n";

		// Loop through the set of WebPages links
		for(it = pagetoword.begin(); it != pagetoword.end(); it++) 
		{
			output << (*it)->getName() << std::endl;
			
		}
	}	
	else
	{
		output << "0\n";
	}
}

void SearchEngine::Lowercase(string& querycommand)
{
	for(unsigned int i = 0; i < querycommand.size(); i++)
	{
		if(isupper(querycommand[i]))
			{
				querycommand[i] = tolower(querycommand[i]);
			}
	}
}
