#include "webpage.h"

using namespace std;

WebPage::WebPage()
{

}

WebPage::WebPage(std::string filename)
{
	name = filename;
	parse();
}

WebPage::~WebPage()
{


}

std::string WebPage::getName()
{
	return name;
}

std::set<std::string> WebPage::getOutLinks()
{
	return outgoing;

}

std::set<std::string> WebPage::getInLinks()
{
	return incoming;

}

std::set<std::string> WebPage::getWords()
{
	return word;
}

void WebPage::setInLinks(std::string filename)
{
	incoming.insert(filename);
}

void WebPage::parse()
{
	// Open ifstream for specific webpage contents
	ifstream files(name);
	char curr_char;
	
	while(files.get(curr_char))
	{
		std::string curr_word;
		std::string linktofile;

		// Adding words to the word set
		if(isalpha(curr_char) || isdigit(curr_char))
		{
			// Convert to lowercase
			if(isupper(curr_char))
			{
				curr_char = tolower(curr_char);
			}

			curr_word = string(1, curr_char);

			// Build the word
			while(files.get(curr_char))
			{

				if(isalpha(curr_char) || isdigit(curr_char))
				{
					if(isupper(curr_char))
					{
						curr_char = tolower(curr_char);
					}
					curr_word += curr_char;
				}

				else
				{
					break;
				}
			}
			// Add the word to the word set
			word.insert(curr_word);
		}

		// Adding links to the outgoing link set
		else if(curr_char == '(')
		{
			files.get(curr_char);
			linktofile = string(1, curr_char);

			while(files.get(curr_char))
			{
				if(curr_char != ')')
				{
					linktofile += curr_char;
				}

				else
				{
					break;
				}
				
			}

			outgoing.insert(linktofile);
		}
	}

	return;
}