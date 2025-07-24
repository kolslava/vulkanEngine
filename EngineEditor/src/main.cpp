#include <EngineCore/Application.hpp>
#include <EngineCore/Logger.hpp>
#include <stdexcept>
#include <iostream>

int main()
{
    // Ініціалізуємо логер
    Log::Init();

    // Створюємо екземпляр нашого додатку
    auto app = new Application();

    try
    {
        // Запускаємо головний цикл
        app->run();
    }
    catch (const std::exception& e)
    {
        // Ловимо та логуємо будь-які помилки
        Log::GetClientLogger()->critical("Unhandled exception: {0}", e.what());
        delete app;
        return EXIT_FAILURE;
    }

    // Звільняємо пам'ять
    delete app;
    return EXIT_SUCCESS;
}