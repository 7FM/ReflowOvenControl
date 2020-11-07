#include <cstdint>
#include <fstream>
#include <iostream>

// Outputs
#define NUMBER_OF_OUTPUTS 6
// Types of outputs
#define TYPE_UNUSED 0
#define TYPE_BOTTOM_ELEMENT 1
#define TYPE_TOP_ELEMENT 2
#define TYPE_BOOST_ELEMENT 3
#define TYPE_CONVECTION_FAN 4
#define TYPE_COOLING_FAN 5

// Number of types
#define NO_OF_TYPES 6

#define isHeatingElement(x) (x == TYPE_TOP_ELEMENT || x == TYPE_BOTTOM_ELEMENT || x == TYPE_BOOST_ELEMENT)

// Learning defines
#define LEARNING_NOT_DONE 0 // Learning has not yet been done
#define LEARNING_DONE 1     // Learning has been done
#define LEARNING_BYPASSED 2 // Learning has been manually bypassed

// Preferences (this can be 1024 bytes maximum).
typedef struct {
    uint16_t sequenceNumber;               // Prefs are rotated between 4 blocks in flash, each 4K in size
    uint8_t outputType[NUMBER_OF_OUTPUTS]; // The type of output for each of the 6 outputs
    uint8_t outputPin[NUMBER_OF_OUTPUTS];
    uint8_t invertedLogicBitMap; // BitMap (assumes <= 8 Outputs) to check for inverted logic (LOW to turn on)
    uint8_t servoPin;
    uint8_t servoOpenDegrees;    // Servo door open
    uint8_t servoClosedDegrees;  // Servo door closed
    uint16_t servoMinPulseWidth; // Servo min pulse width
    uint16_t servoMaxPulseWidth; // Servo max pulse width
    uint8_t learningComplete;    // Have the learning runs been completed?
    uint8_t learnedPower;        // The power (duty cycle) of all elements to keep the oven at 120C
    uint16_t learnedInertia;     // The time need to rasie the temperature from 120C to 150C
    uint16_t learnedInsulation;  // The insulation value of the oven
} __attribute__((packed)) Calibration;

#define STRINGIFY(x) #x

typedef struct _PrintInfo {
    uint8_t bytes;
    const char *outTxt;
    void (*entryCallback)(const char *, const struct _PrintInfo &);
} PrintInfo;

void printU16Hex(const char *, const PrintInfo &);
void printU16Dec(const char *, const PrintInfo &);
void printU8Dec(const char *, const PrintInfo &);
void handlePinConfig(const char *, const PrintInfo &);
void handleLearningState(const char *, const PrintInfo &);

static const constexpr PrintInfo DISAS_INFO[] = {
    {sizeof(uint16_t), "Sequence Number: ", printU16Hex},
    {sizeof(uint8_t) * NUMBER_OF_OUTPUTS * 2 + sizeof(uint8_t), "Pin Configuration: \n", handlePinConfig},
    {sizeof(uint8_t), "Servo Configuration: \n    Servo Pin: ", printU8Dec},
    {sizeof(uint8_t), "    Servo Open Degree: ", printU8Dec},
    {sizeof(uint8_t), "    Servo Close Degree: ", printU8Dec},
    {sizeof(uint16_t), "    Servo Min Pulse Width: ", printU16Dec},
    {sizeof(uint16_t), "    Servo Max Pulse Width: ", printU16Dec},
    {sizeof(uint8_t), "Oven characteristics: \n    Learning State: ", handleLearningState},
    {sizeof(uint8_t), "    Power: ", printU8Dec},
    {sizeof(uint16_t), "    Inertia: ", printU16Dec},
    {sizeof(uint16_t), "    Insulation: ", printU16Dec},
};

int main(int argc, char **argv) {

    if (argc < 2) {
        std::cout << "Expected path to settings file as argument!" << std::endl;
        return 1;
    }

    std::ifstream settingsFile(argv[1], std::ifstream::binary);

    if (!settingsFile) {
        std::cout << "Error could not open specified file: " << argv[1] << std::endl;
        return 2;
    }

    for (size_t i = 0; i < sizeof(DISAS_INFO) / sizeof(DISAS_INFO[0]) && settingsFile; ++i) {
        const PrintInfo &info = DISAS_INFO[i];
        char *buf = new char[info.bytes];

        settingsFile.read(buf, info.bytes);

        std::cout << info.outTxt;

        if (info.entryCallback != nullptr) {
            info.entryCallback(buf, info);
        }
        std::cout << std::endl;

        delete[] buf;
    }

    settingsFile.close();

    return 0;
}

void printU16Hex(const char *buf, const PrintInfo &info) {
    // Data is stored in little endian format!
    uint16_t data = static_cast<uint16_t>(static_cast<uint8_t>(buf[0])) | (static_cast<uint16_t>(static_cast<uint8_t>(buf[1])) << 8);
    std::cout << std::hex << "0x" << static_cast<uint32_t>(data);
}

void printU16Dec(const char *buf, const PrintInfo &info) {
    // Data is stored in little endian format!
    uint16_t data = static_cast<uint16_t>(static_cast<uint8_t>(buf[0])) | (static_cast<uint16_t>(static_cast<uint8_t>(buf[1])) << 8);
    std::cout << std::dec << static_cast<uint32_t>(data);
}

void printU8Dec(const char *buf, const PrintInfo &info) {
    std::cout << std::dec << static_cast<uint32_t>(static_cast<uint8_t>(buf[0]));
}

static const constexpr char *const OUTPUT_TYPE_STR[] = {
    STRINGIFY(TYPE_UNUSED),
    STRINGIFY(TYPE_BOTTOM_ELEMENT),
    STRINGIFY(TYPE_TOP_ELEMENT),
    STRINGIFY(TYPE_BOOST_ELEMENT),
    STRINGIFY(TYPE_CONVECTION_FAN),
    STRINGIFY(TYPE_COOLING_FAN),
};

void handlePinConfig(const char *buf, const PrintInfo &info) {
    const uint8_t *const readBuf = reinterpret_cast<const uint8_t *>(buf);

    const uint8_t invertedBitMap = readBuf[NUMBER_OF_OUTPUTS * 2];

    for (int i = 0; i < NUMBER_OF_OUTPUTS && readBuf[i] != TYPE_UNUSED; ++i) {
        std::cout << "    Output " << std::dec << i << ":" << std::endl;
        uint8_t outputType = readBuf[i];
        std::cout << "      Type: " << (outputType < sizeof(OUTPUT_TYPE_STR) / sizeof(OUTPUT_TYPE_STR[0]) ? OUTPUT_TYPE_STR[outputType] : "INVALID TYPE!") << std::endl;
        uint8_t outputPin = readBuf[NUMBER_OF_OUTPUTS + i];
        std::cout << "      Pin: " << std::dec << static_cast<uint32_t>(outputPin) << std::endl;
        std::cout << "      Inverted: " << std::dec << ((invertedBitMap >> i) & 1);
    }
}

static const constexpr char *const LEARNING_STATE_STR[] = {
    STRINGIFY(LEARNING_NOT_DONE),
    STRINGIFY(LEARNING_DONE),
    STRINGIFY(LEARNING_BYPASSED),
};

void handleLearningState(const char *buf, const PrintInfo &info) {
    const uint8_t *const readBuf = reinterpret_cast<const uint8_t *>(buf);
    uint8_t state = *readBuf;
    std::cout << (state < sizeof(LEARNING_STATE_STR) / sizeof(LEARNING_STATE_STR[0]) ? LEARNING_STATE_STR[state] : "INVALID STATE!");
}
