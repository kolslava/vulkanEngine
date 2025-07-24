#pragma once

// Базовий клас для всіх панелей інтерфейсу
class UIPanel
{
public:
    virtual ~UIPanel() = default;
    virtual void OnImGuiRender() = 0; // Чиста віртуальна функція
};