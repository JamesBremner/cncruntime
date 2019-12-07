#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <iomanip>

#include "wex.h"

using namespace std;
using namespace wex;

/** Extract total elaspsed time from log file
    @param[in] fname path to log file
    @return total seconds in recorded alpased time
*/
int Parse( const std::string& fname )
{
    int total = 0;
    ifstream f( fname );
    if( ! f.is_open() )
    {
        cout << "cannot open " << fname << "\n";
        return total;
    }

    // extract lines from file
    string line;
    while( getline(f, line) )
    {
        // find any elapsed time in line
        int p = line.find("Elapsed: ");
        if( p == -1 )
            continue;
        cout << line << "\n";

        // add to total
        total += atoi( line.substr(p+9).c_str()) * 60 * 60;
        total += atoi( line.substr(p+12).c_str()) * 60;
        total += atoi( line.substr(p+15).c_str());
        cout << "total " << total << "\n";
    }
    return total;
}

int main()
{
    int totalsecs = 0;
    // construct top level window
    gui& form = wex::maker::make();
    form.move({ 50,50,800,600});
    form.text("CNC Run Time Calculator");

    // widget for receiving dropped files
    drop& dropper = wex::maker::make<wex::drop>( form );
    dropper.move( 10,50,790,590 );
    label& instructions = wex::maker::make<wex::label>( dropper );
    instructions.move(30,30,780,580);
    instructions.text("Drop files here");

    // dropped files event handler
    dropper.events().drop( [&](const std::vector<std::string>& files )
    {
        // display list of dropped files
        std::stringstream msg;
        msg << "Files dropped:\n";
        for( auto& f : files )
        {
            std::filesystem::path p( f );
            msg << p.filename().string() << "\n";
        }

        // calculate total elapsed time in all files

        for( auto& f : files )
            totalsecs += Parse( f );
        msg << "\ntotal " <<  std::fixed << std::setprecision(2) << totalsecs/60.0f  << " mins\n";
        msg << "in " << std::to_string( files.size() ) << " logfiles\n";

        // display results
        instructions.text( msg.str() );
        instructions.update();
    });

    button& reset = maker::make<button>( dropper );
    reset.move( 50,0,50,30 );
    reset.text("RESET");
    reset.events().click([&]
    {
        totalsecs = 0;
        instructions.text("Drop files here");
        form.update();
    });

    form.show();
    form.run();

    return 0;
}
