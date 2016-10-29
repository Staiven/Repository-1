
//
//  main.cpp
//  Project 3
//
//  Created by Steven Qu on 10/21/16.
//  Copyright © 2016 Steven Qu. All rights reserved.
//

#include <iostream>
#include <string>
#include <cassert>
using namespace std;
bool hasCorrectSyntax(string pollData);
bool isValidUppercaseStateCode(string stateCode);
int countVotes(string pollData, char party, int& voteCount);
int conversion ( char digit );



int main ()
{
    
    assert(hasCorrectSyntax("TX38RCA55D"));
    assert(!hasCorrectSyntax("MX38RCA55D"));  // state doesn't exist
    assert(!hasCorrectSyntax("TX380CA55D")); // party is a number
    assert(!hasCorrectSyntax("M38RCA55D")); // one letter state
    assert(!hasCorrectSyntax("TX38CA55D")); // no party
    assert(!hasCorrectSyntax("TX38RCA55")); // wrong ending
    assert(!hasCorrectSyntax("TX38RCA")); // wrong endng
    assert(!hasCorrectSyntax("38RCA55D")); // no state code
    assert(!hasCorrectSyntax("TX38%CA55D")); // non letter party
    
    
    
    
    int votes;
    votes = -999;    // so we can detect whether countVotes sets votes
    assert(countVotes("", 'd', votes) == 0  &&  votes == 0);
    votes = -999;    // so we can detect whether countVotes sets votes
    assert(countVotes("TX38RCA55D", '%', votes) == 3  &&  votes == -999);
    votes = -999;    // so we can detect whether countVotes sets votes
    assert(countVotes("TX38R", 'd', votes) == 0  &&  votes == 0);
    votes = -999;    // so we can detect whether countVotes sets votes
    assert(countVotes("TX38R", ' ', votes) == 3  &&  votes == -999);    // non letter party
    votes = -999;    // so we can detect whether countVotes sets votes
    assert(countVotes("TX380", '0', votes) == 1  &&  votes == -999);       // multiple error
    votes = -999;    // so we can detect whether countVotes sets votes
    assert(countVotes("TX38RCA55DMs6rnY29dUT06", 'd', votes) == 1  &&  votes == -999);  // wrong ending time (1)
    votes = -999;    // so we can detect whether countVotes sets votes
    assert(countVotes("TX38RCA55DMs6rnY29dUT", 'd', votes) == 1  &&  votes == -999);    // wrong ending time (2)
    assert(countVotes("TX38RCA55DMs6rnY29dU", 'd', votes) == 1  &&  votes == -999);    // wrong ending time (3)
    votes = -999;    // so we can detect whether countVotes sets votes
    assert(countVotes("TX38RCA55DMs6rnY29dUT00D", 'd', votes) == 2  &&  votes == -999); // two zero
    votes = -999;    // so we can detect whether countVotes sets votes
    assert(countVotes("TX38RCA55DMs6rnY29dUT0D", 'd', votes) == 2  &&  votes == -999);  // one zero
    votes = -999;    // so we can detect whether countVotes sets votes
    assert(countVotes("TX38RCA55DMs6rnY29dUT06D", 'd', votes) == 0  &&  votes == 90);  // correct zero(1)
    votes = -999;    // so we can detect whether countVotes sets votes
    assert(countVotes("TX38RCA55DMs6rnY29dUT60D", 'd', votes) == 0  &&  votes == 144);  // correct zero (2)
    votes = -999;    // so we can detect whether countVotes sets votes
    assert(countVotes("TX38RCA55DMs6rnY29dUX00D", 'd', votes) == 1  &&  votes == -999); // not real state
    votes = -999;    // so we can detect whether countVotes sets votes
    assert(countVotes("TX38RCA55nY29dUT00D", 'd', votes) == 1  &&  votes == -999);  // one letter state
    votes = -999;    // so we can detect whether countVotes sets votes
    assert(countVotes("", '2', votes) == 3  &&  votes == -999);  //non-letter party
    votes = -888;    // so we can detect whether countVotes sets votes
    assert(countVotes("ny29dtx38rhi4d", 'd', votes) == 0  &&  votes == 33);  //non-letter party
    
    
    
    
}



// Return true if the argument is a two-uppercase-letter state code, or
// false otherwise.

bool isValidUppercaseStateCode(string stateCode)
{
    const string codes =
    "AL.AK.AZ.AR.CA.CO.CT.DE.DC.FL.GA.HI.ID.IL.IN.IA.KS."
    "KY.LA.ME.MD.MA.MI.MN.MS.MO.MT.NE.NV.NH.NJ.NM.NY.NC."
    "ND.OH.OK.OR.PA.RI.SC.SD.TN.TX.UT.VT.VA.WA.WV.WI.WY";
    return (stateCode.size() == 2  &&
            stateCode.find('.') == string::npos  &&  // no '.' in stateCode
            codes.find(stateCode) != string::npos);  // match found
}
bool hasCorrectSyntax(string pollData)
{   string result = "";//initate the result string (for checking)
    int clock = 5; // partitioning a string to subset of 4 or 5, if a string ended prematurely, return false
    int space = 1; // (set it to 1 because the first n is 0, helps thinking) change space when the subset has length is 4, keep same space if subset is 5
    for (int n=0; n != pollData.size(); n++)
    {
        result = ""; // reset the result string each time after checking
        if( (n+space)%5 == 1) // check if state is in correct format
        {
            result += toupper(pollData[n]);
            result += toupper(pollData [n+1]);
            clock -= 2;
            if ( isValidUppercaseStateCode(result) == false ) //check if the state is real
                return false;
        }
        if ( (n+space)%5 == 3) // check if the number is in correct format
        {
            if (isdigit(pollData[n]) == false ) // the third entry in a subset has to be number
                return false;
            if ((n+1)<pollData.size())
            {if (isdigit(pollData[n+1]) == false )
            {
                space++; // if the fourth entry is not a number, revise the order accordingly
                clock -= 1;
            }
            else clock -= 2;
            }
        }
        if ((n+space)%5 == 0)
        {
            if (isalpha(pollData[n]) == false)
                return false; // return false if party is not an alphabat
            
            clock = 5;
        }
    }
    
    if (clock != 5)
        return false;
    
    return true;
}


int countVotes(string pollData, char party, int& voteCount)
{   int initialVote = voteCount;
    voteCount = 0;
    int space = 1;
    if (hasCorrectSyntax(pollData) == false) //check syntax
    {   voteCount = initialVote;
        return 1;
    }
    if(isalpha(party) == false)  // check if party is a alphabet
    {   voteCount = initialVote;
        return 3;
    }
    for (int n=0; n != pollData.size(); n++)
    {
        if(n>0 && n< (pollData.size()-1))
        {
            if ((isalpha(pollData[n-1]) == true || pollData[n-1] == '0') && pollData[n] == '0' && isalpha(pollData[n+1]) == true) // (assuming correct format) if an entry preceding a zero is a zero or character, and the entry following the zero is a character, then electoral vote is zero for that state
            {
                voteCount = initialVote;
                return 2;
            }
        }
        
        if ( (n+space)%5 == 3)
        {
            if (isdigit(pollData[n+1]) == false)
                space++;
        }
        
        
        if ( (n+space)%5 == 0 && tolower(pollData[n]) == tolower(party) )
        {
            if (isdigit(pollData[n-2]) == true) // when there are two numbers in a subset
            {
                voteCount = voteCount + conversion(pollData[n-2])*10 + conversion(pollData[n-1]);
                // convert the number to integer format
            }
            else // when there is one number in a subset
            {
                voteCount += conversion(pollData[n-1]); // convert the number to integer format
                
            }
        }
        
        
    }
    return 0;
    
    
}



int conversion ( char digit )
{
    switch ( digit)
    {
        case '0':
            return 0;
            break;
        case '1':
            return 1;
            break;
        case '2':
            return 2;
            break;
        case '3':
            return 3;
            break;
        case '4':
            return 4;
            break;
        case '5':
            return 5;
            break;
        case '6':
            return 6;
            break;
        case '7':
            return 7;
            break;
        case '8':
            return 8;
            break;
        case '9':
            return 9;
            break;
            
    }
    return 0;
}




