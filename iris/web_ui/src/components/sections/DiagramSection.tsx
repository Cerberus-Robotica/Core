import type { DataType } from '../../types';

type Props = {
  data: DataType;
};

export default function DiagramSection({ data }: Props){
    return (
        <>
            <div className="p-4 bg-gray-800 rounded-lg shadow-md">
                <h2 className="text-lg font-bold mb-4">Diagrama</h2>
            </div>
        </>
    );
}