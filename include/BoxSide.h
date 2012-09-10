#pragma once

enum BoxSide {LefSide=1, BotSide=2, BckSide=4, RigSide=8, TopSide=16, FrnSide=32};
const int AllSides = LefSide + BotSide + BckSide + RigSide + TopSide + FrnSide;
const int SideSide = LefSide + BckSide + RigSide + FrnSide;
const int LefRigSide = LefSide + RigSide;
const int BckFrnSide = BckSide + FrnSide;
