import axios from "axios";
import type { DataType } from "../types";

export async function fetchData(): Promise<DataType | null> {
  try {
    const response = await axios.get<DataType>("http://localhost:5000/data");
    return response.data;
  } catch (error) {
    console.error("Erro ao buscar dados:", error);
    return null;
  }
}
