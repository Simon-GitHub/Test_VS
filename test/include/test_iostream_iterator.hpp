#pragma once
#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

int test_iostream_iterator()
{
	istream_iterator< string> is( cin );
	istream_iterator< string> eof;
	vector< string >text;
	copy( is, eof, back_inserter(text) ); // windows:enter ctrl+z	linux/unix:enter ctrl+d Ω· ¯cin ‰»Î
	sort( text.begin(), text.end() );
	ostream_iterator< string> os( cout, " " );
	copy( text.begin(), text.end(), os );
	return 0;
}

int test_iostream_iterator_2()
{
	ifstream in_file( "input_file.txt" );
	ofstream out_file( "output_file.txt" );
	if( !in_file || !out_file )
	{
		cout << "filesopen failed!\n" ;
		return -1;
	}
	istream_iterator< string> is( in_file );
	istream_iterator< string> eof;
	vector< string >text;

	copy( is, eof, back_inserter(text) );
	sort( text.begin(), text.end() );

	ostream_iterator< string> os( out_file, " " );
	copy( text.begin(), text.end(), os );
	return 0;
}
