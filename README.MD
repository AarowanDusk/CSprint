<b>Unreal Engine C++ Assignment Report<b>

This report details the development of a character class system with network functionalities in Unreal Engine using C++.

<b>Core Features:<b>
<ul>
<li>Base Character Class: A reusable C++ class was created to serve as a foundation for various character types like NPCs and the player character. This promotes code organization and simplifies future character development.</li>
<li/>Network Safe CMC (Custom Movement Component): A custom movement component was implemented in C++ with network replication in mind. This ensures smooth character movement across clients even with varying network conditions.</li>
<li>Network Correction and Prediction: The project demonstrates network correction and prediction for high packet lag (300ms), 50% packet loss, and lowered FPS. This mitigates the negative effects of network issues on character movement perceived by players.</li>
<li>Player Input and Game Logic: All player inputs are captured and processed in C++, including those from the player controller. A custom game mode and player state were also created in C++ to manage overall game logic and player-specific data.
C++ to</li>
<li>Blueprint Conversion: The core functionalities developed in C++ were exposed and converted to derived/inherited Blueprints. This allows for easier level design and creation of game assets by designers without needing to directly modify C++ code.</li>
</ul>
<b>Additional Feature:<b>
<ul>
<li>Booster Sprint: A special "booster sprint" functionality was added on top of the standard sprint (activated by holding shift). This one-time boost, activated by right-clicking while sprinting, provides a temporary speed increase. This demonstrates the ability to implement unique gameplay mechanics within the system.</li>
<li>Development Approach:
The project was built entirely from scratch using a blank Unreal Engine project. All code is custom-made in C++ to showcase a deep understanding of the engine's C++ capabilities.</li>
</ul>
This approach offers greater control and flexibility compared to using pre-built assets, allowing for tailored network behavior and unique gameplay features.

This report provides a high-level overview of the project's functionalities.  For a more detailed breakdown, including code snippets or specific implementation details, additional documentation can be provided.
 
