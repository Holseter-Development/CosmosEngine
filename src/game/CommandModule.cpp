#include "CommandModule.h"

CommandModule::CommandModule() : m_Powered(true) {}

void CommandModule::TogglePower() {
    m_Powered = !m_Powered;
}

bool CommandModule::IsPowered() const {
    return m_Powered;
}
