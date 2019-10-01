#include "granular.cpp"

// We make an app.
class ecInterface : public App
{
public:


  // GUI manager for OscEnv voices
  // The name provided determines the name of the directory
  // where the presets and sequences are stored
  //SynthGUIManager<Granular> synthManager {"bin/Granulator"};
  ControlGUI test;
  Granular granulator;
  PresetHandler mPresets{"presets"};
  float background = 0.21;



  OutputRecorder mRecorder;

  virtual void onInit( ) override {

//    mRecorder.start("output.wav", audioIO().framesPerSecond(), 2);
    // Append recorder to audio IO object. This will run the recorder
    // after running the onSound() function below
    audioIO().append(mRecorder);
    //synthManager.setCurrentTab(2); // Run constant synth note.

    //synthManager.triggerOn();

    granulator.init();
    granulator.loadSoundFile("voicePop.wav");
    //granulator.verbose(true);
  }

  virtual void onCreate() override {
    ParameterGUI::initialize();

    //Parameter temp = static_cast<Parameter>(granulator.grainRate);
    //mPresets.registerParameter(temp);
    mPresets 
      << granulator.grainRate << granulator.grainRateLFO << granulator.modGrainRateWidth
      << granulator.asynchronicity << granulator.asyncLFO << granulator.modAsynchronicityWidth
      << granulator.intermittency << granulator.intermittencyLFO << granulator.modIntermittencyWidth
      << granulator.streams << granulator.streamsLFO << granulator.modStreamsWidth 
      << granulator.grainDurationMs << granulator.grainDurationLFO << granulator.modGrainDurationWidth
      << granulator.envelope << granulator.envelopeLFO << granulator.modEnvelopeWidth
      << granulator.tapeHead << granulator.tapeHeadLFO << granulator.modTapeHeadWidth
      << granulator.playbackRate << granulator.playbackRateLFO << granulator.modPlaybackRateWidth
      << granulator.volumeDB << granulator.volumeLFO << granulator.modVolumeWidth
      << granulator.modSineFrequency << granulator.modSinePhase
      << granulator.modSquareFrequency << granulator.modSquareWidth
      << granulator.modSawFrequency << granulator.modSawWidth;

    // Play example sequence. Comment this line to start from scratch
    //        synthManager.synthSequencer().playSequence("synth2.synthSequence");
    //synthManager.synthRecorder().verbose(true);
    //        std::cout << " ----- " << std::endl;

  }

  virtual void onSound(AudioIOData &io) override {
    //synthManager.render(io); // Render audio
    granulator.onProcess(io);
  }

  virtual void onDraw(Graphics &g) override {
    g.clear(background);
    ParameterGUI::beginDraw();
    // synthManager.render(g);

    // // Draw GUI
    // ParameterGUI::beginDraw();
    // ParameterGUI::beginPanel(synthManager.name(),600,-1,-1,600);
    // synthManager.drawFields();
    // ParameterGUI::endPanel();

    // ParameterGUI::beginPanel("Presets",600,600);
    // synthManager.drawPresets();
    // ParameterGUI::endPanel();

    ParameterGUI::beginPanel("Recorder",950,25);
    SoundFileRecordGUI::drawRecorderWidget(&mRecorder, audioIO().framesPerSecond(), audioIO().channelsOut());
    ParameterGUI::endPanel();

    ParameterGUI::beginPanel("LFO Controls", 25, 25,500);
    ParameterGUI::drawParameter(&granulator.modSineFrequency);
    ParameterGUI::drawParameter(&granulator.modSinePhase);
    ParameterGUI::drawParameter(&granulator.modSquareFrequency);
    ParameterGUI::drawParameter(&granulator.modSquareWidth);
    ParameterGUI::drawParameter(&granulator.modSawFrequency);
    ParameterGUI::drawParameter(&granulator.modSawWidth);
    ParameterGUI::endPanel();

    ParameterGUI::beginPanel("Granulator Controls", 675, 250,500,-1);
    ParameterGUI::drawParameter(&granulator.grainRate);
    ParameterGUI::drawParameter(&granulator.asynchronicity);
    ParameterGUI::drawParameter(&granulator.intermittency);
    ParameterGUI::drawParameterInt(&granulator.streams,"");
    ParameterGUI::drawParameter(&granulator.grainDurationMs);
    ParameterGUI::drawParameter(&granulator.envelope);
    ParameterGUI::drawParameter(&granulator.tapeHead);
    ParameterGUI::drawParameter(&granulator.playbackRate);
    ParameterGUI::drawParameter(&granulator.volumeDB);
    
    
    ParameterGUI::endPanel();

    ParameterGUI::beginPanel("Modulation Wave", 525, 250, 150, -1);
    ParameterGUI::drawMenu(&granulator.grainRateLFO);
    ParameterGUI::drawMenu(&granulator.asyncLFO);
    ParameterGUI::drawMenu(&granulator.intermittencyLFO);
    ParameterGUI::drawMenu(&granulator.streamsLFO);
    ParameterGUI::drawMenu(&granulator.grainDurationLFO);
    ParameterGUI::drawMenu(&granulator.envelopeLFO);
    ParameterGUI::drawMenu(&granulator.tapeHeadLFO);
    ParameterGUI::drawMenu(&granulator.playbackRateLFO);
    ParameterGUI::drawMenu(&granulator.volumeLFO);
    ParameterGUI::endPanel();

    ParameterGUI::beginPanel("Modulation Width", 25, 250, 500, -1);
    
    ParameterGUI::drawParameter(&granulator.modGrainRateWidth);
    ParameterGUI::drawParameter(&granulator.modAsynchronicityWidth);
    ParameterGUI::drawParameter(&granulator.modIntermittencyWidth);
    ParameterGUI::drawParameter(&granulator.modStreamsWidth);
    ParameterGUI::drawParameter(&granulator.modGrainDurationWidth);
    ParameterGUI::drawParameter(&granulator.modEnvelopeWidth);
    ParameterGUI::drawParameter(&granulator.modTapeHeadWidth);
    ParameterGUI::drawParameter(&granulator.modPlaybackRateWidth);
    ParameterGUI::drawParameter(&granulator.modVolumeWidth);
    ParameterGUI::endPanel();

    ParameterGUI::beginPanel("Presets", 525, 25);
    ParameterGUI::drawPresetHandler(&mPresets,12,4);
    ParameterGUI::endPanel();

    ImGui::Text("Number of Active Grains: %.1i ",granulator.getActiveVoices() );
    
    ParameterGUI::endDraw();

  }
/*
  virtual void onKeyDown(Keyboard const& k) override {
    if (ParameterGUI::usingKeyboard()) { //Ignore keys if GUI is using them
      return;
    }
    if (k.shift()) {
      // If shift pressed then keyboard sets preset
      int presetNumber = asciiToIndex(k.key());
      synthManager.recallPreset(presetNumber);
    } else {
      // Otherwise trigger note for polyphonic synth
      int midiNote = asciiToMIDI(k.key());

      synthManager.voice()->grainRate.set( ::powf (2.0f, (midiNote - 69.f)/12.f) * 432.f);
      if (midiNote > 0) {
        synthManager.triggerOn(midiNote);
      }
    }
  }

  virtual void onKeyUp(Keyboard const& k) override {
    int midiNote = asciiToMIDI(k.key());
    if (midiNote > 0) {
      synthManager.synth().triggerOff(midiNote);
    }
  }

  void onExit() override {
    // mRecorder.stop();
    ParameterGUI::cleanup();
    

  }
*/

};