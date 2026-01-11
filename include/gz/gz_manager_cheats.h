#ifndef GZ_MANAGER_CHEATS_H
#define GZ_MANAGER_CHEATS_H

class gzCheatsMng_c {
public:
    void execute();
    void executeDisableItemTimer();
    void executeDisableWalls();
    void executeEnableItemTimer();
    void executeEnableWalls();
    void executeInfiniteAir();
    void executeInfiniteArrows();
    void executeInfiniteBombs();
    void executeInfiniteHearts();
    void executeInfiniteOil();
    void executeInfiniteRupees();
    void executeInfiniteSlingshot();
    void executeInvincibleLink();
    void executeInvincibleEnemies();
    void executeMoonJump();
    void executeSuperClawshot();
    void executeUnrestrictedItems();

private:
    // for "run once" reenable checks
    bool mDisableWalls;
    bool mDisableItemTimer;
    bool mSuperClawshot;
};

#endif
