#ifndef COMMAND_MODULE_H
#define COMMAND_MODULE_H

class CommandModule {
public:
    CommandModule();
    void TogglePower();
    bool IsPowered() const;
private:
    bool m_Powered;
};

#endif
