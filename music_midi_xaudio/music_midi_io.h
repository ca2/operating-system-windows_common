#pragma once


using namespace Platform;
using namespace ::winrt::Windows::UI::Xaml::Controls;
using namespace ::winrt::Windows::Devices::Enumeration;
using namespace ::winrt::Windows::Devices::Midi;

namespace SDKSample
{
   namespace MIDI
   {
      /// <summary>
      /// An empty page that can be used on its own or navigated to within a Frame.
      /// </summary>
      [::winrt::Windows::Foundation::Metadata::WebHostHidden]
      public ref class MessageIO sealed
      {
      public:
         MessageIO();
         virtual ~MessageIO();

      protected:
         virtual void OnNavigatedTo(::winrt::Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
         virtual void OnNavigatedFrom(::winrt::Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

      private:
         void ScenarioInit();
         void ScenarioClose();

         void ShowStatusMessage(String^ text);
         void ShowExceptionMessage(Exception^ ex);

         void EnableMessageBuilding();
         void ResetMessageBuilding(bool resetMessageType);
         MidiMessageType GetMessageTypeFromIndex(int index);

         void InPortsListSelectionChanged(Object^ sender, SelectionChangedEventArgs^ e);
         void OutPortsListSelectionChanged(Object^ sender, SelectionChangedEventArgs^ e);
         void MessageTypeSelectionChanged(Object^ sender, SelectionChangedEventArgs^ e);
         void Field1ComboBoxSelectionChanged(Object^ sender, SelectionChangedEventArgs^ e);
         void Field2ComboBoxSelectionChanged(Object^ sender, SelectionChangedEventArgs^ e);
         void Field3ComboBoxSelectionChanged(Object^ sender, SelectionChangedEventArgs^ e);
         void RawMessageTextBlockTapped(Object^ sender, ::winrt::Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
         void ResetButtonClick(Platform::Object^ sender, ::winrt::Windows::UI::Xaml::RoutedEventArgs^ e);
         void SendButtonClick(Platform::Object^ sender, ::winrt::Windows::UI::Xaml::RoutedEventArgs^ e);

         void UpdateMessageTypeUI();
         void UpdateField1UI();
         void UpdateField2UI();
         void UpdateField3UI();

         void OnMessageReceived(MidiInPort ^sender, MidiMessageReceivedEventArgs ^args);

         DeviceInformation^ GetDeviceInformationForInPort(String^ friendlyName);
         DeviceInformation^ GetDeviceInformationForOutPort(String^ friendlyName);

         void CloseInPort(String^ portId);
         void CloseOutPort(String^ portId);

         MainPage^ _rootPage;
         ComboBox^  _messageTypeComboBox;
         ComboBox^ _field1ComboBox;
         ComboBox^ _field2ComboBox;
         ComboBox^ _field3ComboBox;
         TextBlock^ _messageTypeTextBlock;
         TextBlock^ _field1TextBlock;
         TextBlock^ _field2TextBlock;
         TextBlock^ _field3TextBlock;
         TextBlock^ _rawBufferTextBlock;
         ListBox^ _inPortsListBox;
         ListBox^ _outPortsListBox;
         ListBox^ _messageOutputListBox;
         Button^ _sendButton;
         Button^ _resetButton;
         TextBox^ _sendMessageTextBox;

         Platform::Collections::Vector<MidiInPort^> _midiInPortArray;
         Platform::Collections::Vector<IMidiOutPort^> _midiOutPortArray;
         Platform::Collections::Map<MidiMessageType, String ^>^ _messageTypeMap;
         MidiMessageType _messageType;
         int _field1IntValue;
         int _field2IntValue;
         int _field3IntValue;

         ref class MidiDeviceWatcher sealed
         {
         public:
            MidiDeviceWatcher(String ^ midiSelector, ::winrt::Windows::UI::Core::CoreDispatcher ^ dispatcher, ListBox ^ portListBox);
            virtual ~MidiDeviceWatcher();
            void Start();
            void Stop();
            DeviceInformationCollection^ GetDeviceInformationCollection();
            void UpdatePorts();

         private:

            void OnPortAdded(DeviceWatcher^ deviceWatcher, DeviceInformation^ devInfo);
            void OnPortRemoved(DeviceWatcher^ deviceWatcher, DeviceInformationUpdate^ devInfoUpdate);
            void OnPortUpdated(DeviceWatcher^ deviceWatcher, DeviceInformationUpdate^ devInfoUpdate);
            void OnPortEnumCompleted(DeviceWatcher^ deviceWatcher, Object^ obj);

            ::winrt::event_token _portAddedToken;
            ::winrt::event_token _portRemovedToken;
            ::winrt::event_token _portUpdatedToken;
            ::winrt::event_token _portEnumCompleteToken;

            String ^ _midiSelectorString;
            ::winrt::Windows::UI::Core::CoreDispatcher^ _coreDispatcher;
            ListBox ^ _portListBox;
            DeviceWatcher ^ _deviceWatcher;
            DeviceInformationCollection^ _devInfoCollection;
            bool _enumCompleted;
         };

         MidiDeviceWatcher ^ _midiInDeviceWatcher;
         MidiDeviceWatcher ^ _midiOutDeviceWatcher;
      };
   }
}
