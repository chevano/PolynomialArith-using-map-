#include <iostream>
#include <map>
#include <fstream> // use for file handling
#include <sstream> // use for string stream object
#include <vector>

using namespace std;

string printList(int, int, int&);
void simplifyPoly(istringstream&, int&, int&, map<int,int,greater<int> >&, string&, int&);
void setOutput(map<int,int>::iterator&, map<int,int, greater<int> >&, int, int, string&, int&);

int main(int argc, char** argv) {
    // the addtional parameter greater<int> sorts the map in desc order by key
    map<int,int, greater<int> > poly1;
    map<int,int, greater<int> > poly2;
    map<int,int, greater<int> > result;

    vector<string> expressions;
    
    string expression, unprocessPoly1, unprocessPoly2;
    string processPoly1, processPoly2, processResult;
    int coefficient, exponent, counter = 0;

    ifstream inFile;
    inFile.open(argv[1]);
    ofstream outFile(argv[2]);

    while(getline(inFile,expression)) {
        expressions.push_back(expression);
    }

    for(int i = 0; i < expressions.size(); i+=2) {
        unprocessPoly1 = "", unprocessPoly2 = "";

        istringstream polynomial1(expressions[i]);
        simplifyPoly(polynomial1, coefficient, exponent, poly1, unprocessPoly1, counter); 

        istringstream polynomial2(expressions[i+1]);
        simplifyPoly(polynomial2, coefficient, exponent, poly2, unprocessPoly2, counter);

        map<int,int>::iterator it;

        outFile << "Polynomial 1 given" << endl;
        outFile << unprocessPoly1 << endl << endl;

        setOutput(it, poly1, coefficient, exponent, processPoly1, counter);

        outFile << "Polynomial 1 simplified" << endl;
        outFile << processPoly1 << endl << endl;

        outFile << "Polynomial 2 given" << endl;
        outFile << unprocessPoly2 << endl << endl;

        setOutput(it, poly2, coefficient, exponent, processPoly2, counter);

        outFile << "Polynomial 2 simplified" << endl;
        outFile << processPoly2 << endl << endl;

        outFile << "Polynomial 1 + Polynomial 2" << endl;
        result = poly1;

        // Iterates through poly2 and add the values that have the same key as poly1
        // if they don't agree on key then a (key,value) pair will be inserted into result.
        for(it = poly2.begin(); it != poly2.end(); it++)
            result[it->first] += it->second;

        setOutput(it, result, coefficient, exponent, processResult, counter);
        outFile << processResult << endl << endl;
        processResult = "";
       
        outFile << "Polynomial 1 - Polynomial 2" << endl;
        result = poly1;

        // Iterates through poly2 and subtract the values that have the same key as poly1
        // if they don't agree on key then a (key,value) pair will be inserted into result.
        for(it = poly2.begin(); it != poly2.end(); it++)
            result[it->first] -= it->second;

        setOutput(it, result, coefficient, exponent, processResult, counter);
        outFile << processResult << endl << endl;
        processResult = "";
        result.clear();
      
        outFile << "Polynomial 1 * Polynomial 2" << endl;
        
        map<int,int, greater<int> >::iterator it1;
        map<int,int, greater<int> >::iterator it2;
 
        for(it1 = poly1.begin(); it1 != poly1.end(); it1++) {
            for(it2 = poly2.begin(); it2 != poly2.end(); it2++) {

                exponent = it1->first + it2->first;
                coefficient = it1->second * it2->second;

                if(result.count(exponent)) // checks if exponent is already in the map
                    result[exponent] += coefficient; // adds coefficient with the same exponent

                else
                    result.insert(make_pair(exponent,coefficient));
            }
        }

        setOutput(it, result, coefficient, exponent, processResult, counter);
        outFile << processResult << endl << endl << endl;

        processPoly1 = processPoly2 = processResult = "";
        unprocessPoly1 = unprocessPoly2 = "";
        poly1.clear();
        poly2.clear();
        result.clear();
    }

    inFile.close();
    outFile.close();

    return 0;
}

string printList(int coefficient, int exponent, int& counter) {
    string polynomial = "";

    if(counter == 0) {
        
        if(coefficient == 1 && (exponent < 0 || exponent > 1) ) {
            polynomial += "x^" + to_string(exponent);
        }

        else if(coefficient == -1 && (exponent < 0 || exponent > 1) ) {
            polynomial += "-x^" + to_string(exponent);
        }

        else if(coefficient == 1 && exponent == 1) {
            polynomial += "x";
        }

        else if(exponent == 1 && cofficient == -1) {
            polynomial += "-x";
        } 

        else if( (coefficient < 0 || coefficient > 1)
                                && 
            (exponent < 0 || exponent > 1) ) {

            polynomial += to_string(coefficient) + "x^" + to_string(exponent);
        }

        else if(exponent == 0) {

            polynomial += to_string(coefficient);
        } 

        else if( (coefficient < 0 || coefficient > 1) 
                                && 
                (exponent == 1) ) {

            polynomial += to_string(coefficient) + "x";
        }

        else if(coefficient == 0){
            polynomial += "0";
        }
    }
        
    else {

        if(coefficient == 1 && (exponent < 0 || exponent > 1) )
            polynomial += "+x^" + to_string(exponent);

        else if(coefficient == 1 && exponent == 1) {
            polynomial += "+x";
        }

        else if(coefficient == -1 && (exponent < 0 || exponent > 1) ) {
            polynomial += "-x^" + to_string(exponent);
        }

        else if( (coefficient < 0 || coefficient > 1)
                                && 
            (exponent < 0 || exponent > 1) ) {

            if(coefficient < 0) 
                polynomial += to_string(coefficient) + "x^" + to_string(exponent);
            
            else 
                polynomial += "+" + to_string(coefficient) + "x^" + to_string(exponent);
        }

        else if(exponent == 0) {

            if(coefficient < 0) 
                polynomial += to_string(coefficient);
            
            else 
                polynomial += "+" + to_string(coefficient);
            
        }

        else if(exponent == 1 && cofficient == -1) {
            polynomial += "-x";
        } 

        else if( (coefficient < 0 || coefficient > 1) 
                                && 
                (exponent == 1) ) {

            if(coefficient < 0) 
                polynomial += to_string(coefficient) + "x";
        
            else 
                polynomial += "+" + to_string(coefficient) + "x";
        }

        else if(coefficient == 0)
            polynomial += "";
    }

    return polynomial;
}

void simplifyPoly(istringstream& ss, int& coeff, int& exp, map<int,int,greater<int> >& poly, string& unprocessPoly, int& counter) {
    while(ss >> coeff >> exp) {
            if(poly.count(exp)) // checks if exponent is already in the map
                poly[exp] += coeff; // adds coefficient with the same exponent

            else
                poly.insert(make_pair(exp,coeff));
            
            unprocessPoly += printList(coeff, exp, counter);
            counter++;
        }

        ss.clear();
        counter = 0;
}

void setOutput(map<int,int>::iterator& it, map<int,int, greater<int> >&poly, int coeff, int exp, string& processPoly, int& counter) {
    for(it = poly.begin(); it != poly.end(); it++) {
        coeff = it->second;
        exp = it->first;
        processPoly += printList(coeff,exp,counter);
        counter++;
    }
            
    counter = 0;
}
