#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    unsigned char status;
    unsigned char data1;
    unsigned char data2;
} MidiEvent;

void writeMidiFile(const char* filename, MidiEvent* events, int numEvents) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        printf("Failed to create MIDI file.\n");
        return;
    }

    // Write the MIDI file header
    const unsigned char header[] = {0x4D, 0x54, 0x68, 0x64, 0x00, 0x00, 0x00, 0x06,
                                    0x00, 0x00, 0x00, 0x01, 0x00, 0x60};
    fwrite(header, sizeof(header), 1, file);

    // Write the track chunk header
    const unsigned char trackHeader[] = {0x4D, 0x54, 0x72, 0x6B};
    fwrite(trackHeader, sizeof(trackHeader), 1, file);

    // Calculate the track chunk size
    int trackSize = numEvents * sizeof(MidiEvent);
    unsigned char trackSizeBytes[] = {
        (trackSize >> 24) & 0xFF,
        (trackSize >> 16) & 0xFF,
        (trackSize >> 8) & 0xFF,
        trackSize & 0xFF
    };
    fwrite(trackSizeBytes, sizeof(trackSizeBytes), 1, file);

    // Write the MIDI events
    fwrite(events, sizeof(MidiEvent), numEvents, file);

    // Close the MIDI file
    fclose(file);

    printf("MIDI file '%s' created successfully.\n", filename);
}

int main() {
    // Initialize the random number generator
    srand((unsigned int)time(NULL));

    // Generate random MIDI events
    const int numEvents = 10;
    MidiEvent events[numEvents];

    for (int i = 0; i < numEvents; i++) {
        // Generate a random MIDI note event
        unsigned char noteOn = 0x90; // Note On message
        unsigned char noteNumber = rand() % 128;
        unsigned char velocity = rand() % 128;

        events[i].status = noteOn;
        events[i].data1 = noteNumber;
        events[i].data2 = velocity;
    }

    // Write the MIDI events to a file
    writeMidiFile("output.mid", events, numEvents);

    return 0;
}
