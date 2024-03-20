# Course 2 Operating Systems

Курс операционных систем

## Roadmap

- [x] _`T2P1_ThreadCreation`_ **Работа 1** Создание и уничтожение потоков  
- [x] _`T2P2_ThreadSync`_ **Работа 2** Синхронизация потоков с помощью мьютексов, спинлоков и неименованных семафоров  
- [x] _`T2P3_ThreadChannels`_ **Работа 3** Взаимодействие потоков через неименованные каналы  
- [x] _`T2P4_ProcessCreation`_ **Работа 4** Создание и уничтожение процессов  
- [x] _`T2P5_ProcessSync`_ **Работа 5** Синхронизация процессов с помощью именованных семафоров  
- [x] _`T2P6_ProcessSharedMemory`_ **Работа 6** Взаимодействие процессов через разделяемую память  
- [x] _`T2P7_MessageQueues`_ **Работа 7** Взаимодействие процессов в ОС, внутренние коммуникации  
- [x] _`T2P8_ProcessNetwork`_ **Работа 8** Сетевое взаимодействие процессов через сокеты  
- [ ] _`T2P9_OSObjects`_ **Работа 9** Управление доступом к объектам ОС  
- [ ] _`T2P10_ProgramExecution`_ **Работа 10** Специальные механизмы выполнения программ в ОС  

## Compatibility

WSL: `WSL2 Ubuntu 22.04` + `g++ 11.4.0`  
Linux: `Arch Linux 6.7.9` + `g++ 13.2.1`  

| No. | Name | WSL | Linux | Notes |
| - | - | :-: | :-: | - |
| 1 | Создание и уничтожение потоков | ✅ | ✅ |
| 2 | Синхронизация потоков с помощью мьютексов, спинлоков и неименованных семафоров | ✅ | ✅ |
| 3 | Взаимодействие потоков через неименованные каналы | ✅ | ✅ |
| 4 | Создание и уничтожение процессов | ✅ | ✅ |
| 5 | Синхронизация процессов с помощью именованных семафоров | ✅ | ✅ | Unexpected file output in WSL |
| 6 | Взаимодействие процессов через разделяемую память | ✅ | ✅ |
| 7 | Взаимодействие процессов в ОС, внутренние коммуникации | ✅ | ✅ |
| 8 | Сетевое взаимодействие процессов через сокеты | ✅ | ✅ | Server and client do not work in mounted (`/mnt/` on WSL) directories and VBox shared folders |
| 9 | Управление доступом к объектам ОС | ❔ | ❔ |
| 10 | Специальные механизмы выполнения программ в ОС | ❔ | ❔ |