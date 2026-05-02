#ifndef UNIQUE_UTILS_JS
#define UNIQUE_UTILS_JS

const static char utils_js_str[] = R"EOF(
"use strict";

export const BRLCurrencyFormatter = new Intl.NumberFormat(
    "pt-BR",
    {
        style: "currency",
        currency: "BRL",
    }
);

export const BRLDateFormatter = new Intl.DateTimeFormat(
    "pt-BR",
    { dateStyle: 'long' }
);

export const systemColors = {
    secondaryColor20: "hsl(332, 38%, 65%)",
    text: false ? "hsl(0 0 90)" : "hsl(0 0 10)",
    accent: false
        ? "hsl(37, 100%, 64%)"
        : "hsl(37, 100%, 34%)",
    accentTransparent: false
        ? "hsla(37 100 64 / 0.5)"
        : "hsla(37 100 34 / 0.5)",
};

export class BasicIndexedDB {
    #db;

    constructor(dbName, objectStoreNames=[], objectStoreValues=[])
    {
        this.dbName = dbName;
        this.objectStoreNames = objectStoreNames;
        this.objectStoreValues = objectStoreValues;

        if (this.objectStoreNames.length > 0)
        {
            const DBOpenRequest = indexedDB.open(dbName, 1);

            DBOpenRequest.onupgradeneeded = (event) =>
            {
                const db = event.target.result;
 
                db.onerror = () => { console.log("Cannot create database"); }

                for (const objectName of this.objectStoreNames)
                {
                    const objectStore = db.createObjectStore(objectName, { autoIncrement: true });
                    objectStore.createIndex("year", "year", { unique: true });
                }
            }

            if (this.objectStoreValues.length > 0)
            {
                DBOpenRequest.onsuccess = (event) =>
                {
                    this.#db = DBOpenRequest.result;
                    this.#populateDB(this.objectStoreNames, this.objectStoreValues);
                }
            }
        }
    }

    #populateDB(objectStoreNames, objectStoreValues)
    {
        const dbWriteTransaction = this.#db.transaction(objectStoreNames, "readwrite");
        for (let i = 0 ; i < objectStoreNames.length ; i++)
        {
            const objectStore = dbWriteTransaction.objectStore(objectStoreNames[i]);
            for (const timeEntry of objectStoreValues[i])
            {
                const [year, quantity] = Object.entries(timeEntry)[0];
                if (quantity)
                {
                    const insertionRequest = objectStore.add({ year: +year, quantity: +quantity });
                    insertionRequest.onsuccess = (event) =>
                    {
                        console.log("Value inserted in the database");
                    };

                    insertionRequest.onerror = (event) =>
                    {
                        console.log("Unable to add value");
                        console.table({ year: +year, quantity: +quantity });
                    }
                }
            }
        }
    }

    promiseAllData()
    {
        return new Promise((resolve, reject) =>
        {
            const dbData = {};

            const DBOpenRequest = indexedDB.open(this.dbName, 1);
            DBOpenRequest.onsuccess = (event) =>
            {
                this.#db = event.target.result;
                const localObjectStoreNames = Array.from(this.#db.objectStoreNames);
                const dbReadTransaction = this.#db.transaction(localObjectStoreNames, "readonly");
                for (const storeName of localObjectStoreNames)
                {
                    const objectStore = dbReadTransaction.objectStore(storeName);
                    objectStore.getAll().onsuccess = (event) =>
                    {
                        dbData[storeName] = event.target.result;
                    }
                }

                dbReadTransaction.oncomplete = (event) => { resolve(dbData) };
            }
        });
    }

    promiseIsEmpty()
    {
        return new Promise((resolve, reject) =>
        {
            const rowCount = [];

            const DBOpenRequest = indexedDB.open(this.dbName, 1);
            DBOpenRequest.onsuccess = (event) =>
            {
                this.#db = event.target.result;
                const localObjectStoreNames = Array.from(this.#db.objectStoreNames);
                if (localObjectStoreNames.length > 0)
                {
                    const dbReadTransaction = this.#db.transaction(localObjectStoreNames, "readonly");
                    for (const storeName of localObjectStoreNames)
                    {
                        const objectStore = dbReadTransaction.objectStore(storeName);
                        objectStore.count().onsuccess = (event) =>
                        {
                            rowCount.push(event.target.result);
                        }
                    }

                    dbReadTransaction.oncomplete = (event) =>
                    {
                        this.#db.close();
                        resolve(rowCount.every(rowSize => rowSize === 0));
                    }
                }
                else
                {
                    this.#db.close();
                    const deleteRequest = indexedDB.deleteDatabase(this.dbName);
                    deleteRequest.onsuccess = (event) => { resolve(true) };
                }
            }
        });
    }
};
)EOF";

#endif