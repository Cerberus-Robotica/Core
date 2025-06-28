import { useEffect, useRef, useState } from "react";
import type { DataType } from "../types";
import { fetchData } from "./useFetchData"; // já isolado

export function useFetchLoop(start: boolean, initialData: DataType) {
  const [data, setData] = useState<DataType>(initialData);
  const intervalRef = useRef<number | null>(null);

  useEffect(() => {
    if (start) {
      if (intervalRef.current) {
        clearInterval(intervalRef.current);
      }
      intervalRef.current = setInterval(async () => {
        const newData = await fetchData();
        if (newData) {
          setData(newData);
        }
      }, 500) as unknown as number;
    } else if (intervalRef.current) {
      clearInterval(intervalRef.current);
      intervalRef.current = null;
    }

    return () => {
      if (intervalRef.current !== null) {
        clearInterval(intervalRef.current);
      }
    };
  }, [start]);

  return data;
}
