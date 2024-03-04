# Team-010-37-homework4

Team Member Names and x500s:
Vahini Penmatsa (penma013),
Roshan Gokul (gokul010),
Amy Aponchik (aponc005),
Remy Schultz (schu4346)

Demo:
	<video width="320" height="240" controls>
	  <source src="drone-demo_compressed.mp4" type="video/mp4">
	</video>


DockerHub: 
https://hub.docker.com/repository/docker/roshangokul/drone_sim/general

Project overview:
What is the project about and what does the simulation do specifically: 
The simulation is intended to simulate an uber environment. The environment depicts a map of The University of Minnesota Twin-Cities campus. Within the simulation, on the scheduler page, a user creates a name for a robot, selects a pickup and drop off location, and then decides whether the drone will utilize the Astar, Depth First search, or Djikstra algorithm on its path route. In the simulation, the initialized drone will come and pick the robot up and move it to the destination. Multiple trips can be taken with different robots within the same session, so this process repeats for every trip scheduled on the scheduler page. 

The intention of this project is to add two non-trivial features to the basecode of the drone simulation in order to enhance the user experience. In the case of our group, we thought two essential ways to improve the system was to give the Drone battery life and give the simulation a notification system. These two features are pivotal in any transportation system.

How to run the simulation:
The basis for running the simulation relies on utilizing ports and port forwarding. If the simulation is run on a CSE Labs machine, Vole, or using an SSH connection, simply going to the directory the project is located in, using the command “make -j” and then, depending on which port the user wants to utilize, run the command “./build/bin/transit_service (enter port number)/web/”. This will allow the user to run the simulation. To view the simulation, the user can use two links:http://127.0.0.1:(enter port number here) in order to view the simulation, and http://127.0.0.1:(enter port number here)/schedule.html in order to schedule trips. If the user decides to use a local device, then the use of Docker is necessary. In order to run the simulation on Docker, we can simply use the command docker run --rm -it -p (enter port number here):(enter port number here) roshangokul/drone_sim.  Once this command is entered in the local device’s terminal, you can use the two previous links to access the designated webpages.



Feature 1: Battery Feature: 
UML:https://lucid.app/lucidchart/5dfc5d91-5975-4003-b04c-325bf41d85dd/edit?viewport_loc=342%2C-2344%2C1396%2C1726%2C0_0&invitationId=inv_dedec5d6-a1cc-47b4-8518-496c0fc1688c

Our battery feature is automatically implemented within the simulation so there are no user instructions to “use” it. This feature gives the drone a battery while keeping track of battery life based on movement. Whenever the drone moves, its battery will deplete. If the battery life reaches 0, it is assumed to be dead and cannot move nor take on trips. Before a drone is able to go on a trip, it will calculate the euclidean distance of the upcoming trip and see if its battery life will be able to stay positive enough to complete the trip and go to a recharge station if needed. If it is unable to make the trip with its current battery life, before the trip is even taken, the drone will go to the closest of multiple recharger stations in order to fill up the battery. At the recharger station, the drone’s battery will be replenished to be at or near its battery capacity. After this is complete, the drone will continue on with the trip by picking and dropping off the robot. 

In the implementation of our feature, we leveraged two important design patterns - the Factory and Decorator patterns. The factory pattern was implemented to create the recharge stations. It was primarily used to initialize the rechargers on the map, similar to the other mapped entities. The factory pattern was used to do so for the other entities, i.e. the drone factory and the robot factory. Since the recharge stations are of the same object type as the other entities on the map, it made sense to integrate them in a similar fashion, ensuring consistency across the codebase. Another reason why the factory pattern proved to be a useful choice was because it allowed us to create objects of a specific type while keeping the creation logic hidden from the client code. Furthermore, since the recharge stations do not have variations, an abstract factory pattern was deemed unnecessary. Typically, the abstract factory pattern is used when a family of related objects needs to be created together, which was not the case here. 

The decorator pattern was used as a way to wrap the battery onto the drone. Since we simply want to extend the functionality of the drone, a decorator pattern was the best choice. By wrapping the drone with the battery, we are able to add behavior to the drone (such as a battery) without having to modify or adjust the existing drone object. This also allowed us to maintain the open-closed principle by making sure the drone was open for extension but closed for modification. 

The battery and recharger implementation adds to the existing work by allowing the simulation to run as normal, except for the fact that the drone will now have a battery that reduces when it moves. The trip simulation will run as normal except when the battery is running low and is unable to complete the trip and charge without dying. If that is the case, the drone goes to a recharge station instead. When looking at the simulation map, the entire screen looks the same except for the addition of 7 recharge stations that are spread out across the graph/map. This feature allows the system to be more complex and realistic as drones in real life will most likely have a battery and not be able to complete infinite trips without dying. This feature adds to the existing work by providing such a battery and also a method for charging the drones to ensure they are able to complete the trips as normal. The benefits of the feature are further explained below. 

Within all practical situations, when an entity moves,  whether it is a piece of technology or a living creature, it loses energy. Our group wanted the simulation to be as realistic as possible, which entailed giving the drone a battery percentage and the ability to go to charging stations in order to recharge its battery. This is interesting because this allows the resemblance to Uber to be a lot closer as when all uber drivers go on multiple trips, eventually they will need to either fill their car with gas or charge it with electricity.
	
Feature 2: Notifications Feature:

UML:
https://lucid.app/lucidchart/cc65b33a-7bab-4d51-b918-1cc227075b45/edit?viewport_loc=-160%2C67%2C2286%2C1204%2C0_0&invitationId=inv_09e26fd9-936a-45b8-9cd4-d068831f8380

 No special instructions are required to use this new feature as all the necessary instructions on how to run the code are already provided in the README_Project.md. All notifications will automatically be displayed on the simulation screen/page, so the user does not need to do anything to receive the notifications. Our notification feature will display the drone’s behavior on the sidebar of the 3D simulation using the observer pattern. The user will get notified/a message will appear on the left side bar when the drone takes on a trip, when it picks up a robot, when it is delivering the robot, when it has finished the trip and is available for others, when its battery is critical (below 10%), when it is headed to a recharge station, when it has arrived at a recharge station, and when it has finished charging at a recharge station. The user is also notified the current battery percentage of the drone at the beginning of the simulation and when it finishes each trip. 

According to the observer pattern definition, it lets us define a subscription mechanism to notify multiple objects about any events that happen to the object they’re observing. In this case, the drone is the object being observed. The observer will keep track of the drone's behaviors and notify all the subscribers (such as TransitServer) to make sure that they receive real-time updates about the drone. To implement the observer pattern in the existing work, we added an observer object to the drone in order to keep track of any changes that could be made in the drone. Whenever a change occurs, the observer notifies the Publishcer class, which holds a list of subscribers interested in receiving updates about the drone. 
 
The reason why we choose this design pattern to implement the notification system is that it provides real-time updates and is reusable. The pattern not only allows us to track the object itself but also any decorator classes that wrap around it. By using this, we can make sure that the drone’s behavior as well as the battery level changes and corresponding recharger information are received by the subscribers regardless of whether the changes are made by the drone or the decorated battery class that wraps the drone. Also, subscribers can unsubscribe from the updates when they no longer need them, reducing unnecessary code changes. The ability to unsubscribe can be used in client code, so the users themselves are unable to unsubscribe on the simulation page. 

The feature adds to the existing work because users are now able to be notified of the drones location in the trip simulation, along with its battery information. None of the underlying code of the system is changed, but is simply enhanced to include these notifications. Adding this feature and these notifications actually increases the complexity and adds to the existing work of the system because the users are now able to understand the drone’s current state/location without having to inspect the graph/map. This added complexity and feature benefits are further explained below. 

Users are now able to know where the drone is, and whether it is completing a trip or is at a recharge station. This is incredibly helpful and significantly interesting because before, the user would only know where the drone is based off of the simulation map and their own assumptions. Unless they were following the drone on the simulation map closely, the user would not know at what location/stage of the simulation the drone was at. This now allows users to have a better understanding of the drones location, and also for the users to have a bit more transparency about the drones movement/behaviors and the simulation as a whole. It also allows the simulation to resemble apps such as Uber, which usually have notifications that state where along the trip the car is at. 

Sprint Retrospective:

Within the first sprint, Vahini and Roshan worked on the implementation of the recharger stations and battery decorator. Some positives from the sprint involve the usage of effective communication between the two. This included multiple check ups per day on how each partner was doing, whether help was necessary, or if extra help from Teaching Assistants was necessary. In addition to communication just with each other, weekly meetings were set up with Amy in order to check in with each other, and make sure both implementations were making steady progress. But as there are always some upsides to a situation, there were some problems that arose. Being fairly new to Git, Github and Version Control, some errors arose with accidental merging and simply being new to the commands. This resulted in some stints where one partner would have to take a brunt of the work on some days, and vice versa. Overall, being brand new to software design and development, there were some hiccups along the way, but simply being able to communicate with each other whenever there were issues was key in the implementation. 

Within the second sprint, Amy and Remy were supposed to implement the notification system utilizing the observer pattern. After the initial workload assignment, Amy began working on her part. She successfully completed the uml for the notification system and implemented it into the codebase. Amy, Vahini, and Roshan met up twice a week to check in on the progress to make sure everyone was on the same page. Unfortunately, Remy never showed up after the initial workload designation and did not provide updates on his progress despite multiple attempts to reach out to him. As the due date approached and we were running out of time, Amy, Vahini, and Roshan stepped in and decided to take over Remy’s task, ensuring the homework was able to get done on time. Thankfully, Vahini was able to figure it out with only one day remaining.



