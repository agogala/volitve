%% ============================================================
%%   Database name:  BORZA                                     
%%   DBMS name:      Sybase SQL Anywhere                       
%%   Created on:     11.12.97  14:56                           
%% ============================================================

%% ============================================================
%%   Table: STRANKA                                            
%% ============================================================
create table STRANKA
(
    STRANKA_ID      char(10)              not null,
    NAZIV           long varchar                  ,
    EMAIL           long varchar                  ,
    primary key (STRANKA_ID)
);

%% ============================================================
%%   Table: PAPIRJI                                            
%% ============================================================
create table PAPIRJI
(
    PAPIR_ID        char(8)               not null,
    NAZIV           long varchar                  ,
    URL             long varchar                  ,
    primary key (PAPIR_ID)
);

%% ============================================================
%%   Table: ZAHTEVKI                                           
%% ============================================================
create table ZAHTEVKI
(
    STRANKA_ID      char(10)              not null,
    PAPIR_ID        char(8)               not null,
    ZAHTEVEK_ID     integer               not null,
    CENA            numeric(5,2)                  ,
    KOLICINA        integer                       ,
    PAPIR_ZAHTEVKI  char(8)                       ,
    DATUM           date                          ,
    URA             time                          ,
    PONUDNIK        char(10)                      ,
    PREKLIC         numeric(1)                    ,
    primary key (STRANKA_ID, PAPIR_ID, ZAHTEVEK_ID)
);

