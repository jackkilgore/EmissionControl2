/**
 * ecInterface.h
 *
 * AUTHOR: Jack Kilgore
 */

#ifndef ECINTERFACE_H
#define ECINTERFACE_H

/**** Emission Control LIB ****/
#include "ecSynth.h"

/**** AlloLib LIB ****/
#include "al/app/al_App.hpp"
#include "al/io/al_MIDI.hpp"
#include "al/ui/al_ParameterGUI.hpp"
#include "al/ui/al_PresetHandler.hpp"
#include "al_ext/soundfile/al_OutputRecorder.hpp"

/**** External LIB ****/
#include "../external/nativefiledialog/src/include/nfd.h"
#include "imgui_internal.h"

/**** C STD LIB ****/
#include <array>
#include <unordered_set>

class ecInterface : public al::App, public al::MIDIMessageHandler {
 public:
  /**
   * @brief Initilialize the synth interface.
   */
  virtual void onInit() override;

  /**
   * @brief Run once on starup.
   */
  virtual void onCreate() override;

  /**
   * @brief Audio rate processing of synth.
   */
  virtual void onSound(al::AudioIOData &io) override;

  /**
   * @brief Draw rate processing of synth interface.
   */
  virtual void onDraw(al::Graphics &g) override;

  virtual bool onMouseDown(const al::Mouse &m) override;

  /** MIDI Stuff **/
  void initMIDI();
  void updateActiveMIDIParams(const al::MIDIMessage &m);

  /**
   * @brief Called everytime a MIDI message is sent.
   */
  virtual void onMIDIMessage(const al::MIDIMessage &m) override;

  virtual bool onKeyDown(al::Keyboard const &k) override;

  virtual void onExit() override;

  // struct pulled from al_ParameterGUI.hpp for custom preset draw function
  struct PresetHandlerState {
    std::string currentBank;
    int currentBankIndex = 0;
    std::vector<std::string> mapList;
    int presetHandlerBank = 0;
    bool newMap = false;
    std::string enteredText;
    std::string newMapText;
    bool storeButtonState{false};
  };

  // Custom preset draw function (copied and modified from al_ParameterGUI.hpp)
  PresetHandlerState &ECdrawPresetHandler(al::PresetHandler *presetHandler, int presetColumns,
                                          int presetRows);

  /**
   * @brief Modified version of al's soundfilerecordGUI.
   *
   * @param[in] Output recorder object.
   *
   * @param[in] Path of directory where the outputted sound files will be
   * stored.
   *
   * @param[in] Frame rate of outputted file.
   *
   * @param[in] Number of channels of outputted file.
   *
   * @param[in] Amount of space allocated for sound.
   */
  void drawRecorderWidget(al::OutputRecorder *recorder, double frameRate, uint32_t numChannels,
                          std::string directory = "", uint32_t bufferSize = 0);

 private:
  float windowWidth, windowHeight;
  bool isFullScreen;

  bool noSoundFiles, light, isPaused = false, writeSampleRate = false;
  float background = 0.21;
  ecSynth granulator;
  std::unique_ptr<al::PresetHandler> mPresets;
  std::map<int, std::string> currentPresetMap;
  al::OutputRecorder mRecorder;
  Clipper mHardClip;

  KeyDown mLastKeyDown;

  std::array<RtMidiIn, consts::MAX_MIDI_IN> midiIn;
  std::vector<MIDIKey> ActiveMIDI;
  bool mIsLinkingParamAndMIDI = false;
  char mCurrentPresetName[64] = "midi_preset";
  bool allowMIDIPresetOverwrite = false;
  MIDILearnBool mMIDILearn;
  MIDIKey mCurrentLearningMIDIKey;
  std::unordered_set<std::string> MIDIPresetNames;
  std::vector<bool> SelectedMIDIDevices;
  int unlearnFlash = 0;

  void clearActiveMIDI() {
    ActiveMIDI.clear();
    mIsLinkingParamAndMIDI = false;
  }

  /**
   * @brief: Removes all MIDI tied to paramKey in the ActiveMIDI vector.
   */
  void unlinkParamAndMIDI(MIDIKey &paramKey);

  /**
   * @brief: Update ECParameters object at index based on value.
   *
   * @param[in] value: A value between 0 and 1. Percentage of parameter range.
   * @param[in] index: Index in ECParameters structure.
   */
  void updateECParamMIDI(float val, int index) {
    float result = util::outputValInRange(val, granulator.ECParameters[index]->getCurrentMin(),
                                          granulator.ECParameters[index]->getCurrentMax(),
                                          granulator.ECParameters[index]->isLog(), 3);
    granulator.ECParameters[index]->setParam(result);
  }

  /**
   * @brief: Update ECModParameters object at index based on value.
   *
   * @param[in] value: A value between 0 and 1. Percentage of parameter range.
   * @param[in] index: Index in ECModParameters structure.
   */
  void updateECModParamMIDI(float val, int index) {
    float result =
      util::outputValInRange(val, granulator.ECModParameters[index]->param.getCurrentMin(),
                             granulator.ECModParameters[index]->param.getCurrentMax(),
                             granulator.ECModParameters[index]->param.isLog(), 3);
    granulator.ECModParameters[index]->param.setParam(result);
  }

  /**
   * @brief: Update LFOParameters object at index based on value.
   *
   * @param[in] value: A value between 0 and 1. Percentage of parameter range.
   * @param[in] index: Index in LFOParameters structure.
   */
  void updateLFOParamMIDI(float val, int index) {
    float result =
      util::outputValInRange(val, granulator.LFOParameters[index]->frequency->getCurrentMin(),
                             granulator.LFOParameters[index]->frequency->getCurrentMax(),
                             granulator.LFOParameters[index]->frequency->isLog(), 3);
    granulator.LFOParameters[index]->frequency->setParam(result);
  }

  /**
   * @brief: Update duty cycle of LFOParameters object at index based on
   * value.
   *
   * @param[in] value: A value between 0 and 1. Percentage of parameter range.
   * @param[in] index: Index in LFOParameters structure.
   */
  void updateLFODutyParamMIDI(float val, int index) {
    granulator.LFOParameters[index]->duty->set(val);
  }

  std::string soundOutput, execDir, execPath, userPath, configFile, presetsPath, midiPresetsPath;
  nfdchar_t *outPath = NULL;
  nfdpathset_t pathSet;
  nfdresult_t result;
  std::string currentFile = "No file selected";
  std::string previousFile = "No file selected";
  ImFont *bodyFont;
  ImFont *titleFont;
  ImFont *ferrariFont;
  float fontScale = 1.0;
  float adjustScaleY = 1.0;

  double globalSamplingRate = consts::SAMPLE_RATE;

  ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove |
                           ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;
  ImGuiWindowFlags graphFlags = flags;

  int framecounter = 0;
  std::vector<float> streamHistory = std::vector<float>(80, 0);
  int highestStreamCount = 2;
  int grainAccum = 0;
  int grainsPerSecond = 0;
  float oscFrame = 1;
  double lastSamplingRate = globalSamplingRate;

  std::vector<float> oscDataL = std::vector<float>(int(oscFrame *globalSamplingRate), 0);
  std::vector<float> oscDataR = std::vector<float>(int(oscFrame *globalSamplingRate), 0);
  std::vector<float> blackLine = std::vector<float>(2, 0);

  int VUdataSize = globalSamplingRate / 30;
  int lastVUdataSize = VUdataSize;

  std::vector<float> VUdataLeft = std::vector<float>(VUdataSize, 0);
  std::vector<float> VUdataRight = std::vector<float>(VUdataSize, 0);

  std::vector<std::unique_ptr<std::vector<float>>> audioThumbnails;

  float GrainDisplayIndices[consts::MAX_GRAIN_DISPLAY];
  int numGrainsToDisplay;

  // Colors
  int colPushCount = 0;

  // light color scheme
  ImColor PrimaryLight = ImColor(149, 176, 176);  // Background
  ImColor YellowLight = ImColor(237, 224, 39);    // Yellow
  ImColor RedLight = ImColor(212, 35, 89);        // Red
  ImColor GreenLight = ImColor(69, 201, 69);      // Green
  ImColor BlueLight = ImColor(44, 113, 175);      // Blue
  ImColor Shade1Light = ImColor(176, 196, 196);   // Slider Color 1
  ImColor Shade2Light = ImColor(199, 213, 213);   // Slider Color 2
  ImColor Shade3Light = ImColor(221, 230, 230);   // Slider Color 3
  ImColor TextLight = ImColor(0, 0, 0);           // Text Color

  // dark color scheme
  ImColor PrimaryDark = ImColor(33, 38, 40);   // Background
  ImColor YellowDark = ImColor(122, 114, 0);   // Yellow
  ImColor RedDark = ImColor(170, 8, 76);       // Red
  ImColor GreenDark = ImColor(8, 159, 8);      // Green
  ImColor BlueDark = ImColor(15, 75, 129);     // Blue
  ImColor Shade1Dark = ImColor(55, 63, 66);    // Slider Color 1
  ImColor Shade2Dark = ImColor(76, 88, 92);    // Slider Color 2
  ImColor Shade3Dark = ImColor(98, 113, 118);  // Slider Color 3
  ImColor TextDark = ImColor(255, 255, 255);   // Text Color

  ImColor *PrimaryColor;
  ImColor *ECyellow;
  ImColor *ECred;
  ImColor *ECgreen;
  ImColor *ECblue;
  ImColor *Shade1;
  ImColor *Shade2;
  ImColor *Shade3;
  ImColor *Text;

  void drawAudioIO(al::AudioIO *io);

  void setGUIParams();

  int getSampleRateIndex();

  /**** Configuration File Stuff****/

  bool initJsonConfig();

  // These will have dependencies on the userPath member, MAKE SURE TO INIT IT
  // FIRST.˝
  bool jsonWriteSoundOutputPath(std::string path);

  template <typename T>
  bool jsonWriteToConfig(T value, std::string key);

  bool jsonWriteMIDIPresetNames(std::unordered_set<std::string> &presetNames);

  /**
   * @brief Read json config file and write output path to soundOutput member
   * variable.
   *
   *
   */
  json jsonReadConfig();
  void setMIDIPresetNames(json preset_names);
  void setSoundOutputPath(std::string sound_output_path);
  void setAudioSettings(float sample_rate);
  void setColorSchemeMode(bool is_light);
  void setFontScale(float font_scale);
  void setWindowDimensions(float width, float height);
  void setInitFullscreen(bool fullscreen) { isFullScreen = fullscreen; }

  // MIDI Preset Json files
  void writeJSONMIDIPreset(std::string name, bool allowOverwrite);
  void loadJSONMIDIPreset(std::string midi_preset_name);
  void deleteJSONMIDIPreset(std::string midi_preset_name);

  // make a new audioThumbnail when a new sound file is loaded.
  void createAudioThumbnail(float *soundFile, int lengthInSamples);
};

#endif