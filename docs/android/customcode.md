## Introduction


What follows is a detailed tutorial that walks developers through the steps of writing custom code for an Android application using P2UX. Developers should be familiar with the Android Studio integrated development environment for MacOS and the Java programming languages for custom coding. 

Topics include customizable View and control behaviors, Screen/Panels, and UI controls.

!!! note "Terminology"
    Throughout the P2UX documentation the “developer” refers to you, the reader who is developing an app with P2UX. The “designer” refers to the individual whose principal job is to create the appearance and style of the app and its visual components. “PRL” refers to the P2UX Rendering Library for iOS, which controls the creation and workflow of content created for the P2UX platform.

!!! note "Coding examples and code additions"
    In the coding examples below and throughout the documentation, changes and additions to be made by the developer are rendered with a blue highlight.

*P2UX versus the standard Android Studio environment*

P2UX based applications are easy to customize and extend for Android. The P2UX Android rendering library is based on the Android Widget package (android.widget) and other Android primitives. This means that writing custom business logic and integrating custom controls works much the same as it does when writing Android applications using Android Studio.
With the P2UX platform, XML Layout files are no longer required for Android development. Instead, applications use P2UX Portable UX Bundles (PUB) either as embedded or dynamically accessed resources. Additionally, P2UX provides hooks to developers that allow them access to all of the components of P2UX as well as the ability to fully customize instances of the application.

!!! note "Create a P2UX enabled project first"
    This document assumes a P2UX enabled Android Studio project has already been created. See [Quick Start](quickstart.md) for steps to create a project.


## P2UX and Android Studio

*Java*

P2UX native rendering libraries for Android can be extended using the Java programming language. The instructions below provide explicit reference to the proper use of Java coding in each applicable step that follows.

## Adding Custom Behaviors

If desired, customizable behaviors can be added to the project code.

A subclass of the `P2UXAppBehavior` class is the starting point for customizing the behavior of the application. From this class, all other parts of the application can be customized. (Common customized behaviors include creation of a Custom Control or overriding a particular View.)

In Android Studio, create a class to store the customized behaviors. The easiest way to do is by doing a right click on your project package and selecting ==New->Java Class==. (This example uses the name `SampleBehavior`.) In the ==Create New Class== dialog, type in ==P2UXAppBehavior== in the Superclass field.

This will create `SampleBehavior` class. Put any customized behaviors within the implementation here. Below is a list of available customization methods for `P2UXAppBehavior`:

| Modifier and Type | Method and Description |
| --- | --- |
| P2UXScreen  | `createScreen(Context context, P2UXDefinition def, RectF rect, Object index, Object data, P2UXViewContainerDelegate viewDelegate)`<br/><br/>Creates a custom screen |
| P2UXPanel   | `createPanel(Context context, P2UXDefinition def, RectF rect, Object index, Object data, P2UXViewContainerDelegate viewDelegate)`<br/><br/>Creates a custom panel |
| View        | `createControl(String type, P2UXElementInstance elInstance, RectF rect, P2UXViewContainerDelegate viewDelegate, Object index, Object data)`<br/><br/>Creates a custom control |

In addition, update `MainActivity` to include the custom behaviors.

To do this,` MainActivity` must override the method `createBehavior`. (This will override any default behaviors.) Edit `MainActivity` to look like this:

``` Java hl_lines="31 32 33 34 35"
@Override
public void onCreate(Bundle savedInstanceState)
{
   mAppKey = "xxxxx";      
   try {
       mResources = new JSONArray();
       JSONObject resource = new JSONObject();
       resource.put(P2UXAppTypes.P2UXApp_PackageAttrib_FormFactor, 0);
       resource.put(P2UXAppTypes.P2UXApp_PackageAttrib_Type, P2UXAppTypes.P2UXApp_PackageType_Static);
       resource.put(P2UXAppTypes.P2UXApp_PackageAttrib_Package, "shellui_phone");
       resource.put(P2UXAppTypes.P2UXApp_PackageAttrib_Update, P2UXAppTypes.P2UXApp_PackageUpdate_None);
       mResources.put(resource);
   }
   catch (JSONException e)
   {
       P2UXLog.e(TAG, "onCreate - " + e.getMessage());
   }

   if ((getApplicationInfo().flags & ApplicationInfo.FLAG_DEBUGGABLE) == ApplicationInfo.FLAG_DEBUGGABLE) {
       if (savedInstanceState == null) {
           savedInstanceState = new Bundle();
       }
       savedInstanceState.putString(P2UXAppCreator.P2UXAppCreator_Opt_Env, P2UXAppCreator.P2UXAppCreator_Opt_Env_Prototype);
       savedInstanceState.putLong(P2UXAppCreator.P2UXAppCreator_Opt_LogLevel, P2UXLog.P2UXCoreLogFlagVerbose);
   }

   super.onCreate(savedInstanceState);
   applyTranslucentStatusBar();
}

@Override
public P2UXAppBehavior createBehavior(String appId)
{
   return new SampleAppBehavior();
}
```

## Adding Custom Screens/Panels

Developers can override Screen or Panel instances to provide any custom code needed for a hosted `View`.

Since the P2UX UI is not based on XML Layout, connecting custom methods for each button click action is unnecessary. Instead, the developer can override the methods of the `P2UXScreen` class to handle button click and other UI events. (In all other respects, writing code for customized Screen and Panel behavior is the same as for XML layout-based content.)

The PRL will request an instance from `P2UXAppBehavior` when it needs to create a new Screen or Panel. If no instance is provided, P2UX will generate a standard instance of the Screen or Panel.

To provide a custom Screen instance, override the method `createScreen` from a `P2UXAppBehavior` subclass and return an instance of `P2UXScreen`. To provide a custom Panel instance, override the method `createPanel` and return an instance of `P2UXPanel`. (Both `P2UXScreen` and `P2UXPanel` are `View` based subclasses that add some additional helper methods. These methods make it easier to access screen components.)

The name of a chosen Screen or Panel is the value used to identify the item when the PRL creates an instance of this object. The “def” parameter passed to this method contains the member variable `systemType` that identifies the Screen being created. From this object, the developer determines the requested Screen and the specific `P2UXScreen` or `P2UXPanel` subclass to instantiate.

Creating a custom Screen or Panel instance in a `P2UXAppBehavior` subclass

``` Java
@Override
public P2UXScreen createScreen(Context context, P2UXDefinition def, RectF rect, Object index, Object data, P2UXViewContainerDelegate viewDelegate)
{
   if (def.getSystemType().equals("xxxxx")) {
       //return some screen here, for example:
       return new SampleScreen(context, rect, def, viewDelegate, index, data);
   }
   return super.createScreen(context, def, rect, index, data, viewDelegate);
}

@Override
public P2UXPanel createPanel(Context context, P2UXDefinition def, RectF rect, Object index, Object data, P2UXViewContainerDelegate viewDelegate)
{
   if (def.getSystemType().equals("xxxxx")) {
       //return some screen here, for example:
       return new SamplePanel(context, rect, def, viewDelegate, index, data);
   }
   return super.createPanel(context, def, rect, index, data, viewDelegate);
}
```

## Accessing Control instances from `P2UXPanel` or `P2UXScreen`

To access instances of controls, `P2UXScreen` provides a helper method, `controlWithElementSystemTypeOrId`. This method takes the name or id of the control to be accessed and returns a control instance.

“System type” references

After creating a control, the developer can set a “system type” for the control in Builder. This effectively becomes the name for the new control. Referring to the control by its “system type” means that the developer does not need to change any existing custom code when choosing to create an alternate variation of the same control. (Otherwise, if the developer changes the name of the control in Builder, any custom code to referring to the new name would have to be changed as well.)

Accessing a control in a custom instance of `P2UXScreen` looks like this:

``` Java
@Override
public void showView(boolean reload)
{
   super.showView(reload);
   P2UXButton btn = (P2UXButton) controlWithElementSystemTypeOrId("show_btn");
   if (btn != null) {
       btn.setEnabled(false);
   }
}
```

## Handling Events from P2UXAppController

End-users of a completed app will interact with controls in a Screen or Panel to generate events. These interactions will, in turn, trigger actions. Typically the PRL handles events directly through event interactions set up in Builder. In some cases, additional custom code may be needed to trigger actions not directly supported by P2UX.

The PRL provides a chain of event handlers that propagate an event through different components. Handling events at any point in the propagation chain provides a way to trigger custom code and also (optionally) to stop the propagation of the event at any specific handler.

Event propagation follows a set pattern.

First, events are relayed to the `P2UXScreen` for the current Screen or Panel. Next, they pass to `P2UXAppBehavior`. Finally, events are handled by the custom instance of the `View` itself. If none of these components halt event propagation, the PRL will then handle any interactions defined for the event in Builder.

The `P2UXScreen` contains helper methods that handle a variety of events when they occur. These methods give the developer a chance to integrate any custom code needed from the basic event level without triggering any custom actions.

Most standard control events are routed through the `P2UXScreen` instance. Handling these events is a matter of overriding the appropriate event method in your P2UXScreen subclass. Event propagation can be allowed to continue or not depending on the return value from the event handler methods.

The example below illustrates the code that handles a Button press event.

Handling a Button press event in P2UXScreen subclass:

``` Java
@Override
public boolean handleButtonClick(View ctrl, P2UXElementInstance elInstance)
{
   if (elInstance != null && elInstance.getSystemType.equals("button1"))
   {
       String elId = elInstance.getElId();
       if (elId != null && elId.equals("button1")) {
           // take some custom action here for the "okbtn"
           
           // return true to indicate the event was handled and propagation
           // shouldn't continue
           return true;
       }
   }
   return super.handleButtonClick(ctrl);
}
```

##Adding Custom UI Controls

UI Controls are the visual components of a Screen or Panel created in the Builder tool. These can be interactive, like Buttons or Sliders, or non-interactive, like Shapes or Text. Although P2UX offers many different UI Controls directly creatable and usable in Builder, a specific application may require developer-defined UI controls. Builder provides a Custom Control component that can be added to a Screen or Panel and then replaced using custom code.

Any `View` based control can be injected into your Screens and Panels via a Custom Control. This allows for existing custom controls or third party custom controls to be used with P2UX based applications.

The PRL manages the size and position of any custom controls defined using Builder. To provide an instance of a custom control, override the method createControl in your `P2UXAppBehavior` subclass and return the `View` subclassed instance of your control. The first parameter passed to this method, type, is the name of the control set when you created the custom control in Builder. P2UX will pass the initial size to use with your Custom Control, as well as a few other components that can be used to access any custom information created with your control in Builder.

The elementInstance parameter contains all the parameters specified using Builder for the control via its itemSpec member.

Create a custom control in `P2UXAppBehavior` subclass like this:

``` Java
@Override
public View createControl(String type, P2UXElementInstance elemInstance, RectF rect, P2UXViewContainerDelegate viewDelegate, Object index, Object data)
{
   if (type != null && !type.isEmpty()) {
       if (type.equals("calendar")) {

           // Below is an example of implementing a calendar using CalendarView
           View ctrl = new CalendarView(mContext);
           RelativeLayout.LayoutParams params = new RelativeLayout.LayoutParams((int)rect.width(), (int)rect.height());
           params.leftMargin = (int)rect.left;
           params.topMargin = (int)rect.top;
           ctrl.setLayoutParams(params);
           ((CalendarView) ctrl).setOnDateChangeListener(new CalendarView.OnDateChangeListener() {
               @Override
               public void onSelectedDayChange(@NonNull CalendarView view, int year, int month, int dayOfMonth) {
                   Toast.makeText(mContext, month+1 + "/" + dayOfMonth + "/" + year, Toast.LENGTH_SHORT).show();
               }
           });
           return ctrl;
       }
   }
   return super.createControl(type, elemInstance, rect, viewDelegate, index, data);
}
```