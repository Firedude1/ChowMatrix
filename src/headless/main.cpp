#include "../pch.h"
#include "ScreenshotGenerator.h"

String getVersion()
{
    return String (ProjectInfo::projectName) + " - " + ProjectInfo::versionString;
}

String getHelp()
{
    return "ChowMatrix Headless Interface:";
}

int main (int argc, char* argv[])
{
    std::cout << "Running ChowMatrix in headless mode..." << std::endl;

#if JUCE_MAC
    Process::setDockIconVisible (false);    // hide dock icon
#endif
    ScopedJuceInitialiser_GUI scopedJuce;   // creates MessageManager

    ConsoleApplication app;
    app.addVersionCommand ("--version", getVersion());
    app.addHelpCommand ("--help|-h", getHelp(), true);

    ScreenshotGenerator screenshooter;
    app.addCommand (screenshooter);

    return app.findAndRunCommand (argc, argv);
}
