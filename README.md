# decompgz

## Short term goals

- [ ] Understand how framework sets up and executes processes
- [ ] Make a functioning gz process that can run directly under the root layer
- [ ] Compile game for GZ2E01
- [ ] Have the framework execute the gz process on boot then pause (run some logic, draw text to screen)

## Medium term goals

- [ ] Setup basic menu structure
- [ ] Identify a few key features to migrate over
  - [ ] Migrate them and confirm functionality
- [ ] Support compiling game for GZ2P01 and GZ2J01
- [ ] Start thinking about patching strategy that will work for Gamecube and Wii

## Long term goals

- [ ] Come up with spreadsheet of existing features including statuses and target dates for migration
- [ ] Support compiling game for RZDE01_00, RZDE01_02, RZDJ01, RZDP01
- [ ] Implement cross platform patching strategy

## Framework overview

The framework system is responsible for the vast majority of game code execution and drawing to the screen. It does so by constructing a tree of processes organized into layers. Layers are defined by a linked list of node_class structures.

Normal gameplay execution is comprised of 3 layers: the root layer -> scene layer -> room layer.

Each process defines it's own creation, execution, draw and deletion functions in a profile definition which are executed by the framework system.

## Process inheritance structure
```
.
└── base_process_class
    ├── leafdraw_class
    │   ├── fopAc_ac_c
    │   ├── kankyo_class
    │   ├── msg_class
    │   ├── overlap_task_class
    │   └── view_class
    │       └── camera_process_class
    └── process_node_class
        └── scene_class
```

Note this doesn't have anything to do with the framework's structure.

## Open questions

  - How is process priority/order determined?
  - What kind of process should the new gz process be? leafdraw_class base? process_node_class base? new base entirely?
  - At what layer should the gz process execute? Likely directly under root for maximum control
  - How should the gz process profile definition be setup to ensure highest possible execution priority?

## Setup

See [SETUP.md](./SETUP.md)
