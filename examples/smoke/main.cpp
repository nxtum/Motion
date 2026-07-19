import motion;

int main()
{
    static_assert(motion::version_major == 0);
    static_assert(motion::version_minor == 1);
    static_assert(motion::version_patch == 0);

    return 0;
}