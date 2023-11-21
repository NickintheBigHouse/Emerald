#pragma once
#ifndef BASE_EFFECT_MODULE_H
#define BASE_EFFECT_MODULE_H

#include <stdint.h>
#include "daisy_seed.h"
#ifdef __cplusplus

/** @file base_effect_module.h */

using namespace daisy;

namespace bkshepherd
{

// Meta data for an individual Effect Parameter.  Effects may have zero or more Parameters.
// This data structure contains information about the Effect Parameter. 
struct ParameterMetaData
{
  const char* name;           // The Name of this Parameter that gets displayed on the Screen when editing the parameters value
  int valueType;              // The Type of this Parameter value. 0 is raw 0..127 value, 1 is a Float Magnitude (0.0f..1.0f), 2 is Bool (True / False), 3 is a Binned Value of range 1 to valueBinCount
  int valueBinCount;          // The number of distinct choices allowed for this parameter value
  uint8_t defaultValue;       // The Default Value set for this parameter the first time the device is powered up
  int knobMapping;            // The ID of the Physical Knob mapped to this Parameter. -1 if this Parameter is not controlled by a Knob
  int midiCCMapping;          // The Midi CC ID mapped to this Parameter. -1 of this Parameter is not controllable via Midi CC messages
};

class BaseEffectModule
{
  public:
    BaseEffectModule();
    virtual ~BaseEffectModule();

    /** Initializes the module
        \param sample_rate  The sample rate of the audio engine being run.
    */
    virtual void Init(float sample_rate);

    /** Gets the Name of the Effect to display 
        \return Value Name of the Effect
    */
    const char *GetName();

    /** Gets the total number of Effect Parameters
     \return the number of parameters for this effect.
    */
    uint8_t GetParameterCount();

    /** Gets the Name of an Effect Parameter
     \return Value Name of the Effect Parameter
    */
    const char *GetParameterName(int parameter_id);

    /** Gets the Type of an Effect Parameter
     \return an int representing Type of the Effect Parameter. 0 is raw u_int8_t, 1 is Float Magnitude, 2 is Bool, 3 is Binned Int (return -1 is this is unknown)
    */
    int GetParameterType(int parameter_id);

    /** Gets the Bin Count of a Binned Int Effect Parameter
     \return the number of Bins for this Binned Int type Efffect Parameter or -1 if this isn't a Binned Int type parameter
    */
    int GetParameterBinCount(int parameter_id);

    /** Gets the Raw uint8_t value of an Effect Parameter
     \param parameter_id Id of the parameter to retrieve.
     \return the Value (0..127) of the specified parameter.
    */
    uint8_t GetParameterRaw(int parameter_id);

    /** Gets the value of an Effect Parameter as a magnitude mapped to a float 0..1
     \param parameter_id Id of the parameter to retrieve.
     \return the Value of the specified parameter mapped to a float (0..1)
    */
    float GetParameterAsMagnitude(int parameter_id);

    /** Gets the value of an Effect Parameter as a bool (True of False)
     \param parameter_id Id of the parameter to retrieve.
     \return the Value of the specified parameter mapped to True / False
    */
    bool GetParameterAsBool(int parameter_id);

    /** Gets the value of an Effect Parameter as a Binned Integer Value (1..Bin Count)
     \param parameter_id Id of the parameter to retrieve.
     \return the bin number as an int value (1..Bin Count) of the specified parameter
    */
    int GetParameterAsBinnedValue(int parameter_id);

    /** Gets the Parameter ID of the Effect Parameter mapped to Knob. 
     \param knob_id Id of the Knob to retrieve.
     \return the Parameter ID mapped to the specified knob, or -1 if no Parameter is Mapped to that Knob
    */
    int GetMappedParameterIDForKnob(int knob_id);

    /** Gets the Parameter ID of the Effect Parameter mapped to this Midi CC ID. 
     \param midiCC_id Id of the MidiCC to retrieve.
     \return the Parameter ID mapped to the specified MidiCC, or -1 if no Parameter is Mapped to that Midi CC
    */
    int GetMappedParameterIDForMidiCC(int midiCC_id);

    /** Sets the Raw Value for a Particular Effect Parameter.  If the Parameter ID isn't valid, there is no effect.
        \param parameter_id Id of the parameter to set (0 .. m_paramCount - 1).
        \param value Value to set on the parameter.
    */
    void SetParameterRaw(int parameter_id, uint8_t value);

    /** Sets the Value for a Particular Effect Parameter using a float magnitude (0..1).  If the Parameter ID isn't valid, there is no effect.
        \param parameter_id Id of the parameter to set.
        \param value the float magnitude value to set the parameter to.
    */
    void SetParameterAsMagnitude(int parameter_id, float value);

    /** Sets the Value for a Particular Effect Parameter using a bool.  If the Parameter ID isn't valid, there is no effect.
        \param parameter_id Id of the parameter to set.
        \param value the bool value to set the paramter to.
    */
    void SetParameterAsBool(int parameter_id, bool value);

    /** Sets the Value for a Particular Effect Parameter using a Binned Value.  If the Parameter ID isn't valid, there is no effect.
        \param parameter_id Id of the parameter to set.
        \param value the int value (1..Bin Count) of the bin to set the paramter to.
    */
    void SetParameterAsBinnedValue(int parameter_id, u_int8_t bin);

    /** Processes the Effect in Mono for a single sample.  This should only be called once per sample. Also, if this is called, don't call ProcessStereo too.
     \param in Input sample.
    */
    virtual void ProcessMono(float in);

    /** Processes the Effect in Stereo for a single left & right sample.  This should only be called once per sample. Also, if this is called, don't call ProcessMono too.
     \param inL, inR Input sample Left and Right.
    */
    virtual void ProcessStereo(float inL, float inR);

    /**  Gets the most recently calculated Sample Value for the Left Stereo Channel (or Mono)
     \return Last floating point sample for the left channel.
    */
    float GetAudioLeft();

    /** Gets the most recently calculated Sample Value for the Right Stereo Channel
     \return Last floating point sample for the right channel.
    */
    float GetAudioRight();

    /** Returns a value that can be used to drive the Effect LED brightness.
     \return Value 0..1 for the intended LED brightness.
    */
    virtual float GetOutputLEDBrightness();

    /** Sets the state of this Effect
     * @param isActive True for Active, False for Not-Active.
    */
    virtual void SetActive(bool isActive);

    /** Returns the status of the Effect
     \return Value True if the Effect is Active and False if the Effect is Bypassed
    */
    bool IsActive();

    /** Handles updating the custom UI for this Effect.
     * @param elapsedTime a float value of how much time (in seconds) has elapsed since the last update
     */
    virtual void UpdateUI(float elapsedTime);

    /** Handles drawing the custom UI for this Effect.
     * @param display           The display to draw to
     * @param currentIndex      The index in the menu
     * @param numItemsTotal     The total number of items in the menu
     * @param boundsToDrawIn    The Rectangle to draw the item into
     * @param isEditing         True if the enter button was pressed and the value is being edited directly.
    */
    virtual void DrawUI(OneBitGraphicsDisplay& display, int currentIndex, int numItemsTotal, Rectangle boundsToDrawIn, bool isEditing);

  protected:
    /** Initializes the Parameter Storage and creates space for the specified number of stored Effect Parameters
        \param count  The number of stored parameters
    */
    virtual void InitParams(int count);

    const char *m_name;                         // Name of the Effect
    int m_paramCount;                           // Number of Effect Parameters
    uint8_t *m_params;                          // Dynamic Array of Effect Parameter Values
    const ParameterMetaData *m_paramMetaData;   // Dynamic Array of the Meta Data for each Effect Parameter
    float m_audioLeft;                          // Last Audio Sample value for the Left Stereo Channel (or Mono)
    float m_audioRight;                         // Last Audio Sample value for the Right Stereo Channel

  private:
    float   m_isActive;
    float   m_sampleRate;                       // Current Sample Rate this Effect was initialized for.

};
} // namespace bkshepherd
#endif
#endif
