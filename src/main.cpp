#include <iostream>
//#include <sys/mman.h>
#include <gtkmm/main.h>
#include "mainwindow.h"
#include <laterographics/tactileengine.h>
#include <laterographics/audioengine.h>
#include <laterographics/graphics/canvas.h>
#include <boost/program_options.hpp>

#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif

namespace po = boost::program_options;
const float DefaultUpdateRateHz = 750.0;

std::string GetResourcePath() {

    // todo: check if folder exists, look for best location...
    
    std::string rv = "../../resources/";

#ifdef __APPLE__
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
    {
        // error!
    }
    CFRelease(resourcesURL);
    
    rv = path;
    
#endif
    
    return rv;
}

int main(int argc, char *argv[])
{
	// parse command-line parameters

	// description
	po::options_description desc("Options");
	desc.add_options()
    	("haptic-rate", po::value<int>(), "set haptic update rate")
    ;

	// unnamed argument is interpreted as haptic-rate
	po::positional_options_description p;
	p.add("haptic-rate", -1);

	// parse
	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
	po::notify(vm);

	// convert
	if (vm.count("help")) { std::cout << desc << "\n"; return 1; }
	float UpdateRateHz = DefaultUpdateRateHz; // default
	if (vm.count("haptic-rate")) UpdateRateHz = vm["haptic-rate"].as<int>();
	
	latero::Tactograph dev;

	std::cout << "Starting engines...\n";
	latero::graphics::TactileEngine tEngine(&dev, boost::posix_time::microseconds(1E6/UpdateRateHz));
	latero::graphics::AudioEngine aEngine(&dev, boost::posix_time::milliseconds(30));
	tEngine.Start();
	// aEngine.Start(); // we don't need audio
    
    std::vector<std::string> defaultGen;
    defaultGen.push_back("line-horizontal.gen");
    defaultGen.push_back("line-vertical.gen");
    defaultGen.push_back("line-diagonal.gen");
    defaultGen.push_back("wave-slow-horizontal.gen");
    defaultGen.push_back("wave-fast-horizontal.gen");
    defaultGen.push_back("vib-horizontal.gen");
    
    // chdir to Resource path
    std::string path = GetResourcePath();
    chdir(path.c_str());
    
	std::cout << "Creating GUI thread...\n";
	Gtk::Main kit(argc, argv);
    
    MainWindow wnd(&tEngine, &aEngine);
    for (int i=0; i<defaultGen.size(); ++i)
        wnd.AddGenerator(defaultGen[i]);

    Gtk::Main::run(wnd);

	std::cout << "Stopping engines...\n";
	tEngine.Stop();
	aEngine.Stop();

	return 0;
}
