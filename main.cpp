#include <memory>
#include <ranges>
#include <source_location>
#include <string>
#include <variant>
#include <vector>
#include <iostream>


class Device
{
public:
    struct Listener
    {
        virtual ~Listener () = default;
        virtual void update (const std::string & name, const std::variant<int, bool> & value) = 0;
    };

    explicit Device (const std::string & modelName)
        : modelName {modelName}
    {
    }

    void addListener (std::shared_ptr<Listener> listener)
    {
        listeners.push_back (listener);
    }

    void removeListener (std::shared_ptr<Listener> listener)
    {
        listeners.erase (std::ranges::remove (listeners, listener).begin (), listeners.end ());
    }

    std::string getModelName () const
    {
        return modelName;
    }

    void setPreampLevel (int levelDb)
    {
        preampLevelDb = levelDb;
        notifyListeners ("preampLevel", preampLevelDb);
    }

    int getPreampLevel () const
    {
        return preampLevelDb;
    }

    void setPhantomPower(bool state) {
        phantomEnabled = state;
        notifyListeners ("Phantom Power", state);
    }

    bool getPhantomPower() {
        return phantomEnabled;
    }

    static constexpr int MINUS_INFINITY_DB = -127;
    static constexpr int UNITY_GAIN_DB = 0;
    static constexpr int PHANTOM_POWER_V = 48;

private:
    void notifyListeners (const std::string & name, const std::variant<int, bool> & value) const
    {
        for (const auto & listener : listeners)
            listener->update (name, value);
    }

    std::string modelName;
    int preampLevelDb = MINUS_INFINITY_DB;
    bool phantomEnabled = false;

    std::vector<std::shared_ptr<Listener>> listeners;
};


class DeviceMessageGenerator : public Device::Listener
{
public:
    void update (const std::string & name, const std::variant<int, bool> & value) override
    {
        currentMessage = name + " control changed to ";

        if (std::holds_alternative<int> (value))
        {
            const auto intValue = std::get<int> (value);
            currentMessage += std::to_string (intValue);
        }
        else if (std::holds_alternative<bool> (value))
        {
            const auto boolValue = std::get<bool> (value);
            currentMessage += boolValue == true ? "on" : "off";
        }

        std::cout << "Notification: " << currentMessage << "\n";
    }

    std::string currentMessage;
};


std::optional<std::string> findValueString (const std::string & input, const std::string & controlPrefix)
{
    if (! input.starts_with (controlPrefix))
        return std::nullopt;

    auto value = input.substr (controlPrefix.length ());
    value.erase (std::ranges::remove_if (value, isspace).begin (), value.end ());

    return value;
}

bool processDeviceCommand (const std::string & command, Device & device)
{
    if (auto preampLevel = findValueString (command, "set-preamp-level"); preampLevel.has_value ())
    {
        const auto level = std::stoi (*preampLevel);
        device.setPreampLevel (level);
        return true;
    }

    if (auto phantomstate = findValueString(command, "set-phantom-power"); phantomstate.has_value ())
    {
        const auto value = phantomstate.value();

        if (value == "on" || value == "1")
        {
            device.setPhantomPower(true);
            return true;
        }
        if (value == "off" || value == "0")
        {
            device.setPhantomPower(false);
            return true;
        }
    }

    return false;
}


void runApp ()
{
    Device device {"Scarlett 2i2 4th Gen [virtual]"};

    auto messageGenerator = std::make_shared<DeviceMessageGenerator> ();
    device.addListener (messageGenerator);

    std::cout << "DEVICE CONTROL v1.0\n";
    std::cout << "===================\n";
    std::cout << "Connected device: " << device.getModelName () << "\n\n";
    std::cout << "Enter a command followed by a value to set it on the device.\n\n";

    std::cout << "Possible commands\n";
    std::cout << "-----------------\n";
    std::cout << "set-phantom-power [(on, 1)/(off, 0)]     : enable/disable phantom power\n";
    std::cout << "set-preamp-level  [-127 .. 0]            : set the preamp level (dB) \n";
    std::cout << "status                                   : view a list of controls and their values \n";
    std::cout << "quit                                     : quit Device Control \n\n";

    std::string input;

    std::cout << "> ";
    std::getline (std::cin, input);

    while (! input.empty ())
    {
        if (input == "quit")
            break;

        if (input == "status")
        {
            std::cout << "Preamp level: " << std::to_string (device.getPreampLevel ()) << std::endl;

            // Gets the current state (true or false) for Phantom power. Ternary operator changes true/false to on/off for readability
            bool phantomStatus = device.getPhantomPower();
            std::cout << "Phantom Power: " << (phantomStatus ? "on" : "off") << std::endl;
        }
        else if (! processDeviceCommand (input, device))
        {
            std::cout << "Command failed" << std::endl;
        }

        std::cout << "> ";
        std::getline (std::cin, input);
    }
}


// No need to modify the Tester class
struct Tester
{
    void check (bool assertion, const std::source_location & location = std::source_location::current ())
    {
        if (assertion == true)
            return;

        std::cerr << "Test failed at " << location.file_name () << ":" << location.line () << "\n";
        ++numFailures;
    }

    int numFailures = 0;
};

// No need to modify the MockDeviceListener class
struct MockDeviceListener : Device::Listener
{
    void update (const std::string & name, const std::variant<int, bool> & value) override
    {
        ++updateCallCount;
        latestPropertyName = name;
        latestPropertyValue = value;
    }

    std::string latestPropertyName;
    std::variant<int, bool> latestPropertyValue;
    int updateCallCount {0};
};


void testDeviceCanSetPreampLevel (Tester & tester)
{
    Device device {"testDevice"};

    auto listener = std::make_shared<MockDeviceListener> ();
    device.addListener (listener);
    tester.check (listener->updateCallCount == 0);

    device.setPreampLevel (-12);

    tester.check (device.getPreampLevel () == -12);

    tester.check (listener->updateCallCount == 1);
    tester.check (listener->latestPropertyName == "preampLevel");
    tester.check (std::get<int> (listener->latestPropertyValue) == -12);
}

void testMessageGenerator (Tester & tester)
{
    Device device {"testDevice"};

    auto messageGenerator = std::make_shared<DeviceMessageGenerator> ();
    device.addListener (messageGenerator);

    tester.check (messageGenerator->currentMessage.empty ());
    device.setPreampLevel (-6);
    tester.check (messageGenerator->currentMessage == "preampLevel control changed to -6");
}

void testSetPreampLevelCommand (Tester & tester)
{
    const std::string command = "set-preamp-level -6";

    Device device {"testDevice"};
    tester.check (device.getPreampLevel () == Device::MINUS_INFINITY_DB);

    const auto succeeded = processDeviceCommand (command, device);

    tester.check (succeeded == true);
    tester.check (device.getPreampLevel () == -6);
}

void runTests ()
{
    Tester tester;

    testDeviceCanSetPreampLevel (tester);
    testMessageGenerator (tester);
    testSetPreampLevelCommand (tester);

    std::cout << "Number of test failures: " << tester.numFailures
              << (tester.numFailures == 0 ? " :)" : " :(") << "\n";
}


int main (int argc, const char * argv [])
{
    const bool launchedWithTestArg = argc > 1 && std::string (argv [1]) == "--test";

    if (launchedWithTestArg)
        runTests ();
    else
        runApp ();

    return 0;
}
