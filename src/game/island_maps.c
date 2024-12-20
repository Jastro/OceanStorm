int[8][6][6] small_islands =
{
    {
        { 1, 1, 2, 4, 1, 1},
        {22,25,28,31,25, 4},
        { 1, 9,11, 1, 9,11},
        { 1,16,35, 3,28,18},
        { 1, 1,16,17,18, 1},
        { 1, 1, 1, 1, 1, 1}
    },
    {
        { 1, 2, 3, 3, 4, 1},
        { 2,34,10,10,33, 4},
        { 9,10,10,10,10,11},
        {16,27,10,10,10,11},
        { 1,16,27,10,26,18},
        { 1, 1,16,17,18, 1}
    },
    {
        { 1, 1, 1, 1, 2, 4},
        { 1, 1, 2, 3,28,18},
        { 1, 2,34,10,11, 1},
        { 1, 9,10,26,18, 1},
        { 2,28,17,18, 1, 1},
        {16,18, 1, 1, 1, 1},
    },
    {
        {22,25, 3, 3, 4, 1},
        { 1,16,27,10,33, 4},
        { 1, 1, 9,10,10,11},
        { 1, 2,34,10,26,18},
        { 2,34,26,17,18, 1},
        {16,17,18, 1, 1, 1}
    },
    {
        { 1,22,25, 4, 1, 1},
        { 1, 1, 9,33, 3, 4},
        { 1, 2,34,10,26,18},
        { 2,34,26,17,18, 1},
        {16,27,11, 1, 1, 1},
        { 1,16,31,23, 1, 1}
    },
    {
        { 1, 2, 4, 1, 1, 1},
        { 1, 9,33, 4, 1, 1},
        { 2,34,10,33, 4, 1},
        {16,27,10,10,11, 1},
        { 1,16,17,17,35, 4},
        { 1, 1, 1, 1,16,18}
    },
    {
        { 1, 1, 2, 4, 1, 1},
        { 1, 2,34,33, 4, 1},
        { 2,34,10,10,33, 4},
        {16,27,10,10,26,18},
        { 1,16,27,26,18, 1},
        { 1, 1,16,18, 1, 1}
    },
    {
        { 2, 4, 1, 2, 4, 1},
        {16,35, 3,34,33, 4},
        { 1,16,27,10,26,18},
        { 1, 1, 9,10,11, 1},
        { 1, 2,34,10,38,23},
        { 1,16,17,17,18, 1}
    }
};

int[8][9][9] large_islands =
{
    {
        { 1, 1, 2, 3, 4, 1, 1, 1, 1},
        { 1, 2,34,10,33, 3, 4, 1, 1},
        { 1, 9,10,10,10,10,33, 4, 1},
        { 2,34,10,10,10,10,10,33, 4},
        { 9,26,17,27,10,10,10,26,18},
        {16,18, 1, 9,10,10,10,11, 1},
        { 1, 1, 1, 9,10,10,26,18, 1},
        { 1, 1, 2,34,10,26,18, 1, 1},
        { 1, 1,16,17,17,18, 1, 1, 1}
    },
    {
        { 1, 1, 1, 2, 3, 3, 3, 4, 1},
        { 1, 1, 2,34,10,10,10,33, 4},
        { 1, 2,34,10,26,17,27,26,18},
        { 2,34,10,26,18, 1,16,18, 1},
        { 9,10,10,11, 1, 1, 1, 1, 1},
        { 9,10,10,11, 1, 1, 2,24,23},
        {16,27,10,33, 3, 3,34,11, 1},
        { 1,16,17,27,10,26,17,31,23},
        { 1, 1, 1,16,17,18, 1, 1, 1}
    },
    {
        { 1, 1, 1, 2, 3, 4, 1, 1, 1},
        { 1, 1, 2,34,10,33, 4, 1, 1},
        { 1, 1, 9,10,10,10,33, 4, 1},
        { 1, 2,34,10,10,10,10,11, 1},
        { 2,34,10,10,10,10,10,11, 1},
        { 9,26,17,17,27,10,10,33, 4},
        {29,18, 1, 1,16,27,10,10,11},
        {15, 1, 1, 1, 1,16,17,27,11},
        { 1, 1, 1, 1, 1, 1, 1,16,18}
    },
    {
        { 1, 1, 1, 2, 3, 3, 3, 4, 1},
        { 1, 2, 3,34,10,10,26,31,23},
        { 2,34,10,10,26,17,18, 1, 1},
        { 9,10,10,26,18, 1, 1, 1, 8},
        { 9,26,17,18, 1, 1, 2, 3,37},
        {29,18, 1, 1, 1, 2,34,10,11},
        {15, 1, 1, 2, 3,34,10,26,18},
        { 1, 1, 2,34,10,10,26,18, 1},
        { 1, 1,16,17,17,17,18, 1, 1}
    },
    {
        { 1, 1, 1, 1, 1, 2, 3, 4, 1},
        { 1, 1, 1, 1, 2,34,10,33, 4},
        { 1, 1, 2, 3,34,10,10,10,11},
        { 1, 2,34,10,10,10,10,26,18},
        { 1,16,27,10,10,10,26,18, 1},
        { 1, 1, 9,10,10,10,11, 1, 1},
        { 1, 2,34,26,17,27,33, 4, 1},
        { 2,28,17,18, 1,16,27,11, 1},
        {16,18, 1, 1, 1, 1,16,18, 1}
    },
    {
        { 1, 1, 2, 4, 1, 1, 1, 1, 1},
        { 1, 2,34,33, 3, 3, 4, 1, 1},
        { 2,34,10,26,17,27,33, 4, 1},
        {16,27,10,11, 1, 9,10,11, 1},
        { 1, 9,10,11, 1, 9,10,33, 4},
        { 1,16,27,33, 3,34,10,10,11},
        { 1, 1, 9,10,10,10,26,17,18},
        { 1, 1,16,27,10,10,11, 1, 1},
        { 1, 1, 1,16,17,17,18, 1, 1}
    },
    {
        { 2, 3, 4, 1, 1, 1, 1, 1, 1},
        { 9,10,11, 1, 1, 1, 1, 2, 4},
        {16,27,33, 4, 1, 1, 1, 9,11},
        { 1, 9,10,33, 4, 1, 2,34,11},
        { 1,16,27,10,33, 3,34,26,18},
        { 1, 1, 9,10,10,10,10,11, 1},
        { 1, 1, 9,10,10,10,26,18, 1},
        { 1, 2,34,10,10,26,18, 1, 1},
        {22,32,17,17,17,18, 1, 1, 1}
    },
    {
        { 1, 1, 1, 2, 3, 3, 3, 4, 1},
        { 1, 1, 2,34,10,10,10,33, 4},
        { 1, 1, 9,10,10,10,10,10,11},
        { 1, 2,34,10,10,10,10,10,11},
        { 1, 9,10,10,10,10,10,26,18},
        { 2,34,10,10,10,10,26,18, 1},
        {16,27,10,10,10,10,11, 1, 1},
        { 1,16,17,27,10,26,18, 1, 1},
        { 1, 1, 1,16,17,18, 1, 1, 1}
    }
};
