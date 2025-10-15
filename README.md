# Echo Engine - Alpha Version

## Overview
Echo Engine is an early-stage game engine that provides essential rendering and resource management capabilities.

## Features
### Resource Management
- Load various types of assets, including models and textures.

### Rendering
- Display 3D models on the screen.

### Lighting System
Supports multiple light types:
- **Point Light**
- **Directional Light**
- **Spot Light**

### Camera System
- Navigate the scene using the **mouse** and **arrow keys**.
- Zoom in and out with the **mouse scroll**.
- Adjust height using the **spacebar**.

## Project Structure
### External Libraries
The project integrates third-party libraries as well as custom ones. Unit tests for these libraries can be found in the `Tests` folder.

### Assets Folder
The `Assets` directory allows for adding and managing additional resources.

## Installation & Usage
To use Echo Engine, simply clone the repository and add your assets to the `Assets` folder. Ensure all required dependencies are properly installed before running the engine.

---
This is an alpha version; further features and optimizations are in development.

# Echo Engine - Beta Version

Beta Version – Advanced Editor Features
This beta version introduces a more advanced and user-friendly editor designed to streamline your game development workflow.

🧱 Hierarchy & GameObject Creation
Easily create new GameObjects directly from the hierarchy:

Built-in templates: Sphere, Cube, Empty Object, etc.

Objects are displayed in a parent-child hierarchy for better organization and scene structure.

🔍 Inspector
Get a detailed overview of selected GameObjects:

View attached components (e.g., Mesh Renderer, Lights)

Add or remove components dynamically

Modify models and textures in real time

Customize materials directly from the editor

📁 Content Browser
An integrated content explorer allows you to manage your project assets easily:

Import and organize custom resources (models, textures, scripts, etc.)

Supported file types: .obj, .png, .jpg, .h, .cpp

Basic drag & drop support: instantiate GameObjects from models by simply dragging them into the scene

🗂️ Resource & Scene Management
Scenes are automatically saved: your GameObjects and layout persist between sessions

Imported resources are instantly usable across the engine

⚙️ Basic MonoBehaviour System
A lightweight scripting system is included to support custom behaviors on GameObjects, inspired by Unity's MonoBehaviour model.