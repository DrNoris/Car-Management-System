# Car Management System

## Overview
This project is a semester-long Object-Oriented Programming (OOP) class assignment, designed to develop a comprehensive car management system using Qt GUI. The system provides multiple features for managing car data effectively, either stored in memory while the program is running or in a file.

## Features

- **Real-time Data Updates**: Utilizes the Observable design pattern to ensure that all open windows are updated in real-time when changes are made.
- **Table View Model**: Implements a table view model for faster loading and efficient data representation.
- **Data Storage**: Cars can be stored in memory or saved to a file, allowing for flexible data management, based on what kind of repository is instantiated in main.
- **OOP Principles**: Incorporates all four pillars of Object-Oriented Programming (encapsulation, inheritance, polymorphism, and abstraction) throughout the codebase.
- **Statistics Visualization**: Capable of drawing shapes to represent statistical data related to the cars managed by the system.
- **Layered Architecture**: Follows a layered architecture approach, separating concerns between the user interface, business logic, and data access layers for better maintainability and scalability.
- **Main Functional Features**:
  - **Undo**: Revert recent changes made to the car data.
  - **Add**: Easily add new cars to the system.
  - **Delete**: Remove one or multiple cars from the list.
  - **Sort**: Organize cars based on various attributes like model, year, or plate number.
  - **Filter**: Filter the displayed list of cars based on specific criteria.
  - **Find**: Quickly search for a car using its attributes.
- **Car Wash List Generator/Bucket Features**:
  - **Add**: Can add an existing car to a bucket.
  - **Random Generate**: Generates a bucket with a given number of random existing cars.
  - **Empty**: Erase all the data from the bucket.
  - **Export**: Exports the cars from the bucket to a HTML file directly on the user's desktop. The file opens in the web-browser and it comes with CSS.
## Technologies Used
- C++
- Qt Framework
- Object-Oriented Programming Concepts
- HTML, CSS
- Standard Template Library (STL)
