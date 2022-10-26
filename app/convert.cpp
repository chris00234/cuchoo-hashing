#include "convert.hpp"
#include "Wordset.hpp"
#include <iostream>
#include <set>
#include <sstream>
#include <map>
#include <stack>
#include <queue>
#include <vector>
#include <string>
#include <utility>

// You should not need to change this function.
void loadWordsIntoTable(WordSet& words, std::istream& in)
{
	std::string line, word;
	std::stringstream ss;

	while (getline(in, line))
	{
		ss.clear();
		ss << line;
		while (ss >> word)
		{
			words.insert(word);
		}
	}

}

std::vector< std::string > convert(const std::string& s1, const std::string& s2, const WordSet& words)
{
    std::vector< std::string > ret;
    if (s1 == s2)
    { // Case 1: (s1 == s2). No need to do a bfs.
        ret.push_back(s1);
        return ret;
    }
    if (s1.size() != s2.size()) // Case 2: both words have different sizes
        return ret;
    if(!words.contains(s1) || !words.contains(s2)) // Case 3: words are not in the wordSet.
        return ret;
    
    
    std::queue< std::string > temp_queue;
    std::queue< std::string > bfs_queue;
    std::set< std::string > discovered;
    std::stack< std::pair<std::string, std::string> > path;
    std::pair< std::string, std::string > edge = {s1,""}; // first value should be actual node, second one should be value before.
    std::string m_s1, m_s2;
    
    path.push(edge);
    bfs_queue.push(s1);

    while (!bfs_queue.empty())
    {
        m_s1 = bfs_queue.front();
        discovered.insert(m_s1);
        bfs_queue.pop();
        temp_queue = helperFunction(m_s1, words, discovered);
        while (!temp_queue.empty())
        {
            m_s2 = temp_queue.front();
            if (m_s2 == s2)
            {
                m_s2 = m_s1;
                std::pair< std::string, std::string > tmp_edge;
                std::stack< std::string > tmp_path;
                tmp_path.push(s2);
                while(!path.empty()){
                    tmp_edge = path.top();
                    path.pop();
                    if(tmp_edge.first == m_s2){
                        tmp_path.push(tmp_edge.first);
                        m_s2 = tmp_edge.second;
                    }
                }
                while(!tmp_path.empty()){
                    ret.push_back(tmp_path.top());
                    tmp_path.pop();
                }
                // start popping values from Stack and return.
//                ret.push_back(s1_copy);
                return ret;
            }
            temp_queue.pop();
            edge = {m_s2, m_s1};
            path.push(edge);
            bfs_queue.push(m_s2);
        }
    }
    return ret;  // stub obviously
}

std::queue< std::string > helperFunction(const std::string& s1, const WordSet& words, std::set< std::string > &discovered)
{
    std::queue < std::string > temp;
    std::string s1_copy = s1;
    char init;

    for (int a = 0; a != s1.size(); a++)
    {
        init = 'a'; // iterates from 'a' to 'z'.
        for (int i = 0; i < 26; i++)
        {
            s1_copy[a] = init++;
            if (words.contains(s1_copy) && discovered.count(s1_copy) != 1)
            {
                temp.push(s1_copy);
                discovered.insert(s1_copy);
            }
            s1_copy = s1;
        }
    }
    return temp;
}

//
//BFS(startV) {
//   Enqueue startV in frontierQueue
//   Add startV to discoveredSet
//
//   while ( frontierQueue is not empty ) {
//      currentV = Dequeue from frontierQueue
//      "Visit" currentV
//      for each vertex adjV adjacent to currentV {
//         if ( adjV is not in discoveredSet ) {
//            Enqueue adjV in frontierQueue
//            Add adjV to discoveredSet
//         }
//      }
//   }
//}
