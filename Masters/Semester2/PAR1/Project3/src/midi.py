import pandas as pd
from mido import Message, MidiFile, MidiTrack

# Load your 5D C++ Harmony Search data
df = pd.read_csv("output.csv", encoding="utf-16")

# Create a new MIDI file and track
mid = MidiFile()
track = MidiTrack()
mid.tracks.append(track)

# We will play each newly found harmony for 200 ticks (about half a second)
time_per_chord = 200

for index, row in df.iterrows():
    # Read all 5 instruments (variables) for the DMFC problem
    note1 = int(row["Note1_MIDI"])
    note2 = int(row["Note2_MIDI"])
    note3 = int(row["Note3_MIDI"])
    note4 = int(row["Note4_MIDI"])
    note5 = int(row["Note5_MIDI"])

    # "Note on" commands: All 5 instruments start playing their notes simultaneously (delta time = 0)
    track.append(Message("note_on", note=note1, velocity=64, time=0))
    track.append(Message("note_on", note=note2, velocity=64, time=0))
    track.append(Message("note_on", note=note3, velocity=64, time=0))
    track.append(Message("note_on", note=note4, velocity=64, time=0))
    track.append(Message("note_on", note=note5, velocity=64, time=0))

    # "Note off" commands: The first instrument waits for 'time_per_chord' ticks before stopping.
    # The others stop immediately after the first one stops (delta time = 0).
    track.append(Message("note_off", note=note1, velocity=64, time=time_per_chord))
    track.append(Message("note_off", note=note2, velocity=64, time=0))
    track.append(Message("note_off", note=note3, velocity=64, time=0))
    track.append(Message("note_off", note=note4, velocity=64, time=0))
    track.append(Message("note_off", note=note5, velocity=64, time=0))

# Save the resulting melody to a file
mid.save("optimization_melody.mid")
print("MIDI file saved successfully! You can now play 'optimization_melody.mid'.")