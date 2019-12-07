#include <iostream>
#include <fstream>
#include "wex.h"

using namespace std;
using namespace wex;

int Parse( const std::string& fname )
{
    int total = 0;
    ifstream f( fname );
    if( ! f.is_open() ) {
        cout << "cannot open " << fname << "\n";
        return total;
    }
    string line;
    while( getline(f, line) )
    {
        int p = line.find("Elapsed: ");
        if( p == -1 )
            continue;
        cout << line << "\n";
        total += atoi( line.substr(p+9).c_str()) * 60 * 60;
        total += atoi( line.substr(p+12).c_str()) * 60;
        total += atoi( line.substr(p+15).c_str());
        cout << "total " << total << "\n";
    }
    return total;
}

int main()
{
    // construct top level window
    gui& form = wex::maker::make();
    form.move({ 50,50,800,600});
    form.text("CNC Run Time Calculator");

    // widget for receiving dropped files
    drop& dropper = wex::maker::make<wex::drop>( form );
    dropper.move( 10,10,790,590 );
    label& instructions = wex::maker::make<wex::label>( dropper );
    instructions.move(30,30,780,580);
    instructions.text("Drop files here");

    // dropped files event handler
    dropper.events().drop( [&](const std::vector<std::string>& files )
    {
        // display list of dropped files
        std::string msg;
        msg = "Files dropped:\n";
        for( auto& f : files )
            msg += f + "\n";
        int total = 0;
        for( auto& f : files )
            total += Parse( f );
        msg += "total " + std::to_string( total ) + " secs\n";
        instructions.text( msg );
        instructions.update();
    });

    form.show();
    form.run();

    return 0;
}
